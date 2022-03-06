#pragma once

#include <QtCore>

#include "interfaces/IConnection.hpp"
#include "pan_protocol/pan_protocol_lib.h"
#include "util/SocketUtil.hpp"

class PanguConnection : public QObject, IConnection
{
    Q_OBJECT

public:
    PanguConnection(QObject *parent, const QString &serverName = "localhost",
                    const int &serverPort = 10363);
    ~PanguConnection() override;

    ConnectionErr connect() override;
    ConnectionErr disconnect() override;

    ConnectionErr sendCommand(std::unique_ptr<ParsedCommand> &command,
                              unsigned char *&img,
                              unsigned long &size) override;
    ConnectionErr sendCommand(std::unique_ptr<ParsedCommand> &command) override;

private:
    QString serverName_;
    int serverPort_;

    SOCKET sock_;
};
