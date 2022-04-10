#pragma once

#include <QtCore>

#include "enums/ConnectionErr.hpp"

class IConnection
{
public:
    virtual ~IConnection()
    {
    }

    virtual ConnectionErr connect(const QString &address, const int &port) = 0;
    virtual ConnectionErr disconnect() = 0;

    virtual ConnectionErr ping() = 0;

    virtual void connected() = 0;
    virtual void disconnected() = 0;
};
