#pragma once

#include <QtCore>

#include "common/ParseResult.hpp"

/**
 * Parser interface
 */
class IParser
{
public:
    /**
     * Virtual destructor
     */
    virtual ~IParser()
    {
    }

    /**
     * Parse a command
     * @param strCommand String to parse
     * @return The parsed result
     */
    virtual ParseResult parse(const QString &strCommand) = 0;
};
