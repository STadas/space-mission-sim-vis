#include "ParseResult.hpp"

ParseResult::ParseResult(CommandErr err, ParsedCommand command)
    : err(err)
    , command(command)
{
}

ParseResult::ParseResult(CommandErr err)
    : err(err)
{
}
