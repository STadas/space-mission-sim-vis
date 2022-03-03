#pragma once

#include <QtCore>

#include "pan_protocol/socket_stuff.h"

class SocketUtil
{
public:
    static unsigned long hostNameToAddr(const QString &s);

private:
    SocketUtil(){};
};
