#pragma once

#include <QtCore>

#include "common/ParsedCommand.hpp"
#include "enums/ConnectionErr.hpp"

class ICommandSender
{
public:
    virtual ~ICommandSender()
    {
    }

    virtual ConnectionErr sendCommand(ParsedCommand &command) = 0;
    virtual ConnectionErr sendCommand(ParsedCommand &command,
                                      unsigned char *&img,
                                      unsigned long &size) = 0;
};
