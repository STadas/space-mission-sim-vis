#pragma once

#include <QtCore>

#include "enums/ConnectionErr.hpp"

/**
 * Connection interface
 */
class IConnection
{
public:
    /**
     * Virtual destructor
     */
    virtual ~IConnection()
    {
    }

    /**
     * Connect to a server
     * @param address Address of the server
     * @param port Port of the server
     * @return Error status
     */
    virtual ConnectionErr connect(const QString &address, const int &port) = 0;
    /**
     * Disconnect from the current server
     * @return Error status
     */
    virtual ConnectionErr disconnect() = 0;

    /**
     * Ping the current server
     * @return Error status
     */
    virtual ConnectionErr ping() = 0;

    /**
     * Emitted when there is a successful interaction with the server
     */
    virtual void connected() = 0;
    /**
     * Emitted when there is an unsuccessful interaction with the server
     */
    virtual void disconnected() = 0;
};
