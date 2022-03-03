#pragma once

#include <QtCore>
#include <variant>

typedef std::variant<std::string, long, double> Arg;

class ParsedCommand
{
public:
    ParsedCommand(QString name, bool expectsImg,
                  std::vector<std::shared_ptr<Arg>> args);

    bool expectsImg() const
    {
        return this->expectsImg_;
    };

    QString name() const
    {
        return this->name_;
    }
    std::vector<std::shared_ptr<Arg>> args() const
    {
        return this->args_;
    }

private:
    QString name_;
    bool expectsImg_;
    std::vector<std::shared_ptr<Arg>> args_;
};
