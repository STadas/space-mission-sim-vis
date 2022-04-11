#pragma once

#include <QtCore>

#include "pan_protocol/socket_stuff.h"

/**
 * Socket utilities class
 */
class SocketUtil
{
public:
    /**
     * Convert host string to an address accepted by sockets
     * @param s Host string
     * @return Address accepted by sockets
     */
    static unsigned long hostNameToAddr(const QString &s);

private:
    /**
     * Private constructor
     */
    SocketUtil()
    {
    }
};
