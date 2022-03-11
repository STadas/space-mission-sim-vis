#pragma once

#include <QtCore>

#include "common/ParsedCommand.hpp"
#include "enums/ConnectionErr.hpp"

class ICommandSender
{
public:
    ~ICommandSender()
    {
    }

    virtual ConnectionErr sendCommand(
        std::unique_ptr<ParsedCommand> &command) = 0;
    virtual ConnectionErr sendCommand(std::unique_ptr<ParsedCommand> &command,
                                      unsigned char *&img,
                                      unsigned long &size) = 0;
};
