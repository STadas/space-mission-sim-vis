#pragma once

#include <QtCore>
#include <variant>

typedef std::variant<std::string, long, double> Arg;

class ParsedCommand
{
public:
    ParsedCommand(QString name, bool expectsImg, std::vector<Arg> args);

    bool expectsImg() const
    {
        return this->expectsImg_;
    };

    QString name() const
    {
        return this->name_;
    }
    std::vector<Arg> args() const
    {
        return this->args_;
    }

private:
    QString name_;
    bool expectsImg_;
    std::vector<Arg> args_;
};
