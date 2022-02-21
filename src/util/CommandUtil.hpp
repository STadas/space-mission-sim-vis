#pragma once

#include <QtCore>

#include "common/ParsedCommand.hpp"
#include "enums/CommandErr.hpp"
#include "util/StringUtil.hpp"

class CommandUtil
{
public:
    static CommandErr parsePangu(const QString &strCommand,
                                 std::unique_ptr<ParsedCommand> &parsedCommand);

private:
    CommandUtil(){};
};
