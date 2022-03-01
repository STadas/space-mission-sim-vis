#include "ParsedCommand.hpp"

ParsedCommand::ParsedCommand(QString name, bool expectsImg,
                             std::vector<std::shared_ptr<Arg>> args)
    : name_(name)
    , expectsImg_(expectsImg)
    , args_(args)
{
}
