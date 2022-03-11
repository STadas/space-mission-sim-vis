#include "PanguConnection.hpp"

PanguConnection::PanguConnection(QObject *parent, const QString &serverName,
                                 const int &serverPort)
    : QObject(parent)
    , serverName_(serverName)
    , serverPort_(serverPort)
{
}

PanguConnection::~PanguConnection()
{
}

ConnectionErr PanguConnection::connect()
{
    /* Modified example provided with PANGU */

    unsigned long addr;
    size_t saddr_len;

    struct sockaddr_in saddr;

#ifdef _WIN32
    WSAData wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData))
    {
        return ConnectionErr::BAD_SOCK;
    }
#endif

    addr = SocketUtil::hostNameToAddr(this->serverName_);
    this->sock_ = socket(AF_INET, SOCK_STREAM, 0);

    if (this->sock_ == -1)
    {
        return ConnectionErr::BAD_SOCK;
    }

    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = addr;
    saddr.sin_port = htons(this->serverPort_);
    saddr_len = sizeof(struct sockaddr_in);

    if (::connect(this->sock_, (struct sockaddr *)&saddr, saddr_len) == -1)
    {
        return ConnectionErr::BAD_CONN;
    }

    char *panguErr = pan_net_start_TX(this->sock_);
    if (panguErr)
    {
        return ConnectionErr::BAD_RESPONSE;
    }

    return ConnectionErr::OK;
}

ConnectionErr PanguConnection::disconnect()
{
    /* Modified example provided with PANGU */

    pan_net_finish_TX(this->sock_);
    SOCKET_CLOSE(this->sock_);

#ifdef _WIN32
    WSACleanup();
#endif
    return ConnectionErr::OK;
}

ConnectionErr PanguConnection::sendCommand(
    std::unique_ptr<ParsedCommand> &command)
{
    /* try-catch does have a performance cost (and +4 spaces) but it's probably
     * more readable and maintainable than repeating what's already there in
     * CommandUtil */

    try
    {
        char *panguErr;
        if (command->name() == "start")
        {
            panguErr = pan_net_set_viewpoint_by_degrees_d_TX(
                this->sock_, std::get<double>(command->args()[0]),
                std::get<double>(command->args()[1]),
                std::get<double>(command->args()[2]),
                std::get<double>(command->args()[3]),
                std::get<double>(command->args()[4]),
                std::get<double>(command->args()[5]));

            if (panguErr)
            {
                return ConnectionErr::BAD_RESPONSE;
            }

            return ConnectionErr::OK;
        }

        if (command->name() == "quaternion")
        {
            panguErr = pan_net_set_viewpoint_by_quaternion_d_TX(
                this->sock_, std::get<double>(command->args()[0]),
                std::get<double>(command->args()[1]),
                std::get<double>(command->args()[2]),
                std::get<double>(command->args()[3]),
                std::get<double>(command->args()[4]),
                std::get<double>(command->args()[5]),
                std::get<double>(command->args()[6]));

            if (panguErr)
            {
                return ConnectionErr::BAD_RESPONSE;
            }

            return ConnectionErr::OK;
        }
    }
    catch (std::bad_variant_access)
    {
    }

    return ConnectionErr::BAD_REQUEST;
}

ConnectionErr PanguConnection::sendCommand(
    std::unique_ptr<ParsedCommand> &command, unsigned char *&img,
    unsigned long &size)
{
    ConnectionErr err = this->sendCommand(command);
    if (err != ConnectionErr::OK)
    {
        return err;
    }

    if (!command->expectsImg())
    {
        return ConnectionErr::OK;
    }

    char *panguErr = pan_net_get_image_TX(this->sock_);
    if (panguErr)
    {
        return ConnectionErr::BAD_RESPONSE;
    }

    img = pan_net_get_image_RX(this->sock_, &size);
    if (img == nullptr)
    {
        return ConnectionErr::BAD_DATA;
    }

    return ConnectionErr::OK;
}
