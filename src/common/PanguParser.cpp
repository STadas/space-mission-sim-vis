#include "PanguParser.hpp"

std::map<QString, PanguParser::CommandName> PanguParser::commandMap = {
    {"start", CommandName::Start},
    {"quaternion", CommandName::Quaternion},
    {"update", CommandName::Update},
    {"pause", CommandName::Pause},
};

PanguParser::PanguParser(QObject *parent)
    : QObject(parent)
{
}

PanguParser::~PanguParser()
{
}

ParseResult PanguParser::parse(const QString &strCommand)
{
    QString strippedCommand = strCommand.split(QRegExp("#+"))[0];
    QStringList words =
        strippedCommand.split(QRegExp("[ \\t]+"), Qt::SkipEmptyParts);

    if (words.size() == 0 || (words.size() == 1 && words[0].size() == 0))
        return CommandErr::EMPTY;

    std::vector<Arg> args;

    QString cmdName = words[0];
    words.erase(words.begin());

    if (PanguParser::commandMap.find(cmdName) == PanguParser::commandMap.end())
    {
        return CommandErr::NOT_IMPLEMENTED;
    }

    switch (PanguParser::commandMap[cmdName])
    {
        case CommandName::Start: {
            if (words.size() != 6)
                return CommandErr::BAD_ARG_COUNT;

            for (auto &it : words)
            {
                if (!StringUtil::isNumeric(it))
                    return CommandErr::BAD_ARG_TYPE;

                args.push_back(Arg(it.toDouble()));
            }

            return {CommandErr::OK, ParsedCommand(cmdName, true, args)};
        }

        case CommandName::Quaternion: {
            if (words.size() != 7)
                return CommandErr::BAD_ARG_COUNT;

            for (auto &it : words)
            {
                if (!StringUtil::isNumeric(it))
                    return CommandErr::BAD_ARG_TYPE;

                args.push_back(Arg(it.toDouble()));
            }

            return {CommandErr::OK, ParsedCommand(cmdName, true, args)};
        }

        case CommandName::Update: {
            if (words.size() != 0)
                return CommandErr::BAD_ARG_COUNT;

            return {CommandErr::OK, ParsedCommand(cmdName, true, args)};
        }

        case CommandName::Pause: {
            if (words.size() != 1)
                return CommandErr::BAD_ARG_COUNT;

            if (!StringUtil::isNumeric(words[0]))
                return CommandErr::BAD_ARG_TYPE;

            args.push_back(Arg(words[0].toDouble()));

            return {CommandErr::OK, ParsedCommand(cmdName, false, args)};
        }

        default: {
            return CommandErr::UNKNOWN;
        }
    }
}
