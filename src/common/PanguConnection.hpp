#pragma once

#include <QtCore>

#include "common/PanguParser.hpp"
#include "interfaces/ICommandSender.hpp"
#include "interfaces/IConnection.hpp"
#include "pan_protocol/pan_protocol_lib.h"
#include "util/SocketUtil.hpp"

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

    ConnectionErr ping() override;

    ConnectionErr sendCommand(ParsedCommand &command, unsigned char *&img,
                              unsigned long &size) override;
    ConnectionErr sendCommand(ParsedCommand &command) override;

signals:
    void connected() override;
    void disconnected() override;

private:
    SOCKET sock_;
};
