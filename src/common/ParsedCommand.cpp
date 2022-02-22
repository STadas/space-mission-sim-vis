#include "ParsedCommand.hpp"

ParsedCommand::ParsedCommand(QString name, bool expectsImage,
                             std::vector<std::shared_ptr<Arg>> args)
    : name_(name)
    , expectsImage_(expectsImage)
    , args_(args)
{
}
