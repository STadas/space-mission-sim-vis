#include "CommandUtil.hpp"
#include "common/ParsedCommand.hpp"

CommandErr CommandUtil::parsePangu(
    const QString &strCommand, std::unique_ptr<ParsedCommand> &parsedCommand)
{
    std::vector<QString> words = StringUtil::split(strCommand, ' ');
    std::vector<std::shared_ptr<Arg>> args{};

    if (words.size() == 0)
        return CommandErr::EMPTY;

    QString cmdName = words[0];
    words.erase(words.begin());

    if (cmdName == QString("start"))
    {
        if (words.size() != 6)
            return CommandErr::BAD_ARG_COUNT;

        for (auto &it : words)
        {
            if (!StringUtil::isNumeric(it))
                return CommandErr::BAD_ARG_TYPE;

            args.push_back(std::make_shared<Arg>(std::stod(it.toStdString())));
        }

        parsedCommand = std::make_unique<ParsedCommand>(cmdName, true, args);

        return CommandErr::OK;
    }

    if (cmdName == QString("quaternion"))
    {
        if (words.size() != 7)
            return CommandErr::BAD_ARG_COUNT;

        for (auto &it : words)
        {
            if (!StringUtil::isNumeric(it))
                return CommandErr::BAD_ARG_TYPE;

            args.push_back(std::make_shared<Arg>(std::stod(it.toStdString())));
        }

        parsedCommand = std::make_unique<ParsedCommand>(cmdName, true, args);

        return CommandErr::OK;
    }

    return CommandErr::NOT_IMPLEMENTED;
}
