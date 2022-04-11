#pragma once

#include <QtCore>
#include <QtWidgets>
#include <variant>

typedef std::variant<std::string, long, double> Arg;

/**
 * Parsed command class. Stores parsed information of a command like its name,
 * arguments, etc.
 */
class ParsedCommand
{
public:
    /**
     * Constructor
     * @param name The name of the command
     * @param args Argument list of the command
     * @param expectsImg Whether the command expects an image back from the
     * server
     */
    ParsedCommand(QString name, std::vector<Arg> args, bool expectsImg);
    /**
     * Constructor
     * @param name The name of the command
     * @param args Argument list of the command
     * @param expectsImg Whether the command expects an image back from the
     * server
     * @param camPos The resulting camera position from the command
     */
    ParsedCommand(QString name, std::vector<Arg> args, bool expectsImg,
                  QVector3D camPos);

    /**
     * @return Whether the command expects an image
     */
    bool expectsImg() const;
    /**
     * @return Whether the command has a resulting camera position
     */
    bool hasCamPos() const;

    /**
     * @return Command name
     */
    QString name() const;
    /**
     * @return Command argument list
     */
    std::vector<Arg> args() const;
    /**
     * @return Command resulting camera position
     */
    QVector3D camPos() const;

private:
    QString name_;
    std::vector<Arg> args_;
    QVector3D camPos_;

    bool expectsImg_;
    bool hasCamPos_;
};
