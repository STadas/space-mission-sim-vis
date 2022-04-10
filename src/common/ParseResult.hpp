#pragma once

#include <optional>
#include "common/ParsedCommand.hpp"
#include "enums/CommandErr.hpp"

class ParseResult
{
public:
    ParseResult(CommandErr err, ParsedCommand command);
    ParseResult(CommandErr err);

    CommandErr err;
    std::optional<ParsedCommand> command;

private:
    ParseResult();
};
