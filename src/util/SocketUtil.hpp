#pragma once

#include "pan_protocol/socket_stuff.h"

#include <QtCore>

class SocketUtil
{
public:
    static unsigned long hostNameToAddr(const QString &s);

private:
    SocketUtil(){};
};
