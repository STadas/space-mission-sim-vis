#pragma once

#include <QtCore>

#include "interfaces/IParser.hpp"
#include "util/StringUtil.hpp"

class PanguParser : public QObject, IParser
{
    Q_OBJECT

public:
    PanguParser(QObject *parent = nullptr);
    ~PanguParser();
    CommandErr parse(const QString &strCommand,
                     std::unique_ptr<ParsedCommand> &parsedCommand) override;
};
