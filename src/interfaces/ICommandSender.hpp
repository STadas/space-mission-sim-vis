#pragma once

#include <QtCore>

#include "common/ParsedCommand.hpp"
#include "enums/ConnectionErr.hpp"

/**
 * Command sender interface
 */
class ICommandSender
{
public:
    /**
     * Virtual destructor
     */
    virtual ~ICommandSender()
    {
    }

    /**
     * Send a command
     * @param command The parsed command to send
     * @return Error status
     */
    virtual ConnectionErr sendCommand(ParsedCommand &command) = 0;
    /**
     * Send a command
     * @param command The parsed command to send
     * @param img Memory address to put the image bytes to
     * @param size Variable to update with the size of the image
     * @return Error status
     */
    virtual ConnectionErr sendCommand(ParsedCommand &command,
                                      unsigned char *&img,
                                      unsigned long &size) = 0;
};
