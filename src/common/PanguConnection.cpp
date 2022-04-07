#include "PanguConnection.hpp"

PanguConnection::PanguConnection(QObject *parent)
    : QObject(parent)
{
}

PanguConnection::~PanguConnection()
{
}

ConnectionErr PanguConnection::connect(const QString &address, const int &port)
{
    /* Modified example provided with PANGU */

    emit this->disconnected();

    unsigned long addr;
    size_t saddr_len;

    struct sockaddr_in saddr;

#ifdef _WIN32
    WSAData wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData))
    {
        return ConnectionErr::BadSock;
    }
#endif

    addr = SocketUtil::hostNameToAddr(address);
    this->sock_ = socket(AF_INET, SOCK_STREAM, 0);

    if (this->sock_ == -1)
    {
        return ConnectionErr::BadSock;
    }

    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = addr;
    saddr.sin_port = htons(port);
    saddr_len = sizeof(struct sockaddr_in);

    if (::connect(this->sock_, (struct sockaddr *)&saddr, saddr_len) == -1)
    {
        return ConnectionErr::BadConn;
    }

    char *panguErr = pan_net_start_TX(this->sock_);
    if (panguErr)
    {
        return ConnectionErr::BadResponse;
    }

    emit this->connected();

    return ConnectionErr::Ok;
}

ConnectionErr PanguConnection::disconnect()
{
    /* Modified example provided with PANGU */

    pan_net_finish_TX(this->sock_);
    SOCKET_CLOSE(this->sock_);

#ifdef _WIN32
    WSACleanup();
#endif

    emit this->disconnected();
    return ConnectionErr::Ok;
}

ConnectionErr PanguConnection::ping()
{
    unsigned char outPayload[4] = {1, 2, 3, 4};
    unsigned long outSize = 4;
    char *panguErr = pan_net_echo_TX(this->sock_, &outPayload, outSize);

    if (panguErr)
    {
        emit this->disconnected();
        return ConnectionErr::BadResponse;
    }

    unsigned long inSize = 0;
    unsigned char *inPayload =
        (unsigned char *)pan_net_echo_RX(this->sock_, &inSize);

    if (outSize != inSize)
    {
        free(inPayload);
        emit this->disconnected();
        return ConnectionErr::BadData;
    }

    for (int i = 0; i < inSize; ++i)
    {
        if (outPayload[i] != inPayload[i])
        {
            free(inPayload);
            emit this->disconnected();
            return ConnectionErr::BadData;
        }
    }

    free(inPayload);
    emit this->connected();
    return ConnectionErr::Ok;
}

ConnectionErr PanguConnection::sendCommand(ParsedCommand &command)
{
    /* try-catch does have a performance cost (and +4 spaces) but it's probably
     * more readable and maintainable than repeating even more of what's already
     * there in PanguParser */

    QString cmdName = command.name();
    if (PanguParser::commandMap.find(cmdName) == PanguParser::commandMap.end())
    {
        return ConnectionErr::BadRequest;
    }

    try
    {
        char *panguErr;
        switch (PanguParser::commandMap[command.name()])
        {
            case PanguParser::CommandName::Start: {
                panguErr = pan_net_set_viewpoint_by_degrees_d_TX(
                    this->sock_, std::get<double>(command.args()[0]),
                    std::get<double>(command.args()[1]),
                    std::get<double>(command.args()[2]),
                    std::get<double>(command.args()[3]),
                    std::get<double>(command.args()[4]),
                    std::get<double>(command.args()[5]));

                if (panguErr)
                {
                    emit this->disconnected();
                    return ConnectionErr::BadResponse;
                }

                return ConnectionErr::Ok;
            }

            case PanguParser::CommandName::Quaternion: {
                panguErr = pan_net_set_viewpoint_by_quaternion_d_TX(
                    this->sock_, std::get<double>(command.args()[0]),
                    std::get<double>(command.args()[1]),
                    std::get<double>(command.args()[2]),
                    std::get<double>(command.args()[3]),
                    std::get<double>(command.args()[4]),
                    std::get<double>(command.args()[5]),
                    std::get<double>(command.args()[6]));

                if (panguErr)
                {
                    emit this->disconnected();
                    return ConnectionErr::BadResponse;
                }

                return ConnectionErr::Ok;
            }

            case PanguParser::CommandName::Update: {
                return ConnectionErr::Ok;
            }

            case PanguParser::CommandName::Pause: {
                QThread::currentThread()->msleep(
                    1000 * std::get<double>(command.args()[0]));
                return ConnectionErr::Ok;
            }
        }
    }
    catch (std::bad_variant_access)
    {
    }

    return ConnectionErr::BadRequest;
}

ConnectionErr PanguConnection::sendCommand(ParsedCommand &command,
                                           unsigned char *&img,
                                           unsigned long &size)
{
    ConnectionErr err = this->sendCommand(command);
    if (err != ConnectionErr::Ok)
    {
        return err;
    }

    if (!command.expectsImg())
    {
        return ConnectionErr::Ok;
    }

    char *panguErr = pan_net_get_image_TX(this->sock_);
    if (panguErr)
    {
        emit this->disconnected();
        return ConnectionErr::BadResponse;
    }

    img = pan_net_get_image_RX(this->sock_, &size);
    if (img == nullptr)
    {
        return ConnectionErr::BadData;
    }

    return ConnectionErr::Ok;
}
