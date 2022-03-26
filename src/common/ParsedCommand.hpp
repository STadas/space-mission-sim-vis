#pragma once

#include <QtCore>
#include <QtWidgets>
#include <variant>

typedef std::variant<std::string, long, double> Arg;

class ParsedCommand
{
public:
    ParsedCommand(QString name, std::vector<Arg> args, bool expectsImg);
    ParsedCommand(QString name, std::vector<Arg> args, bool expectsImg,
                  QVector3D camPos);

    bool expectsImg() const;
    bool hasCamPos() const;

    QString name() const;
    std::vector<Arg> args() const;
    QVector3D camPos() const;

private:
    QString name_;
    std::vector<Arg> args_;
    QVector3D camPos_;

    bool expectsImg_;
    bool hasCamPos_;
};
