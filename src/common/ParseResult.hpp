#pragma once

#include <optional>
#include "common/ParsedCommand.hpp"
#include "enums/CommandErr.hpp"

/**
 * Parse result class. Holds the parse error status and the parsed command
 */
class ParseResult
{
public:
    /**
     * Constructor
     * @param err Parse error status
     * @param command Parsed command
     */
    ParseResult(CommandErr err, ParsedCommand command);
    /**
     * Constructor
     * @param err Parse error status
     */
    ParseResult(CommandErr err);

    CommandErr err;
    std::optional<ParsedCommand> command;

private:
    /**
     * Private default constructor
     */
    ParseResult();
};
