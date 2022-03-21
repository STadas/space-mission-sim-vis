#pragma once

#include <QtCore>

#include "interfaces/ICommandSender.hpp"
#include "interfaces/IConnection.hpp"
#include "pan_protocol/pan_protocol_lib.h"
#include "util/SocketUtil.hpp"
#include "common/PanguParser.hpp"

class PanguConnection : public QObject,
                        public IConnection,
                        public ICommandSender
{
    Q_OBJECT

public:
    PanguConnection(QObject *parent);
    ~PanguConnection() override;

    ConnectionErr connect(const QString &address, const int &port) override;
    ConnectionErr disconnect() override;

    ConnectionErr sendCommand(ParsedCommand &command, unsigned char *&img,
                              unsigned long &size) override;
    ConnectionErr sendCommand(ParsedCommand &command) override;

private:
    SOCKET sock_;
};
