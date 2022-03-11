#pragma once

#include "common/ParsedCommand.hpp"
#include "enums/CommandErr.hpp"

class IParser
{
public:
    ~IParser()
    {
    }

    virtual CommandErr parse(const QString &strCommand,
                             std::unique_ptr<ParsedCommand> &parsedCommand) = 0;
};
