#include "ParsedCommand.hpp"

ParsedCommand::ParsedCommand(QString name, std::vector<Arg> args,
                             bool expectsImg)
    : name_(name)
    , args_(args)
    , expectsImg_(expectsImg)
{
}

ParsedCommand::ParsedCommand(QString name, std::vector<Arg> args,
                             bool expectsImg, QVector3D camPos)
    : name_(name)
    , args_(args)
    , camPos_(camPos)
    , expectsImg_(expectsImg)
{
}

bool ParsedCommand::expectsImg() const
{
    return this->expectsImg_;
}

bool ParsedCommand::hasCamPos() const
{
    return this->hasCamPos_;
}

QString ParsedCommand::name() const
{
    return this->name_;
}

std::vector<Arg> ParsedCommand::args() const
{
    return this->args_;
}

QVector3D ParsedCommand::camPos() const
{
    return this->camPos_;
}
