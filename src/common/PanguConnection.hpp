#pragma once

#include <QtCore>

#include "common/PanguParser.hpp"
#include "interfaces/ICommandSender.hpp"
#include "interfaces/IConnection.hpp"
#include "pan_protocol/pan_protocol_lib.h"
#include "util/SocketUtil.hpp"

/**
 * PANGU connection class. Controls the connection to a PANGU server and sends
 * commands to it
 */
class PanguConnection : public QObject,
                        public IConnection,
                        public ICommandSender
{
    Q_OBJECT

public:
    /**
     * Constructor
     * @param parent Parent object
     */
    PanguConnection(QObject *parent);
    /**
     * Destructor
     */
    ~PanguConnection() override;

    /**
     * Connect to a server
     * @param address Address of the server
     * @param port Port of the server
     * @return Error status
     */
    ConnectionErr connect(const QString &address, const int &port) override;
    /**
     * Disconnect from the current server
     * @return Error status
     */
    ConnectionErr disconnect() override;

    /**
     * Ping the current server
     * @return Error status
     */
    ConnectionErr ping() override;

    /**
     * Send a command to the current server, expecting an image back
     * @param command The parsed command to send
     * @param img Memory address to put the image bytes to
     * @param size Variable to update with the size of the image
     * @return Error status
     */
    ConnectionErr sendCommand(ParsedCommand &command, unsigned char *&img,
                              unsigned long &size) override;
    /**
     * Send a command to the current server
     * @param command The parsed command to send
     * @return Error status
     */
    ConnectionErr sendCommand(ParsedCommand &command) override;

signals:
    /**
     * Emitted when there is a successful interaction with the server
     */
    void connected() override;
    /**
     * Emitted when there is an unsuccessful interaction with the server
     */
    void disconnected() override;

private:
    SOCKET sock_;
};
