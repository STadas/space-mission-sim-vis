#pragma once

#include <QtCore>

#include "common/ParsedCommand.hpp"
#include "enums/ConnectionErr.hpp"

class IConnection
{
public:
    virtual ~IConnection(){};

    virtual ConnectionErr connect() = 0;
    virtual ConnectionErr disconnect() = 0;
    virtual ConnectionErr sendCommand(const ParsedCommand &command) = 0;
    virtual ConnectionErr sendCommand(const ParsedCommand &command,
                                      unsigned char *&img,
                                      unsigned long &size) = 0;
};
