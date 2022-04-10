#pragma once

#include <optional>
#include "common/ParsedCommand.hpp"
#include "enums/CommandErr.hpp"

struct ParseResult {
public:
    ParseResult(CommandErr err, ParsedCommand command)
        : err(err)
        , command(command)
    {
    }

    ParseResult(CommandErr err)
        : err(err)
    {
    }

    CommandErr err;
    std::optional<ParsedCommand> command;

private:
    ParseResult();
};

class IParser
{
public:
    virtual ~IParser()
    {
    }

    virtual ParseResult parse(const QString &strCommand) = 0;
};
