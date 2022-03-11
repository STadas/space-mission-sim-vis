#pragma once

#include <QtCore>

#include "common/ParsedCommand.hpp"
#include "enums/ConnectionErr.hpp"

class IConnection
{
public:
    virtual ~IConnection()
    {
    }

    virtual ConnectionErr connect() = 0;
    virtual ConnectionErr disconnect() = 0;
    virtual ConnectionErr sendCommand(
        std::unique_ptr<ParsedCommand> &command) = 0;
    virtual ConnectionErr sendCommand(std::unique_ptr<ParsedCommand> &command,
                                      unsigned char *&img,
                                      unsigned long &size) = 0;
};
