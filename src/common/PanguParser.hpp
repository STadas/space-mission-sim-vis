#pragma once

#include <QtCore>

#include "interfaces/IParser.hpp"
#include "util/StringUtil.hpp"

class PanguParser : public QObject, public IParser
{
    Q_OBJECT

public:
    PanguParser(QObject *parent = nullptr);
    ~PanguParser();
    ParseResult parse(const QString &strCommand) override;

    enum class CommandName {
        Start,
        Quaternion,
        Update,
        Pause,
        SetTime,
    };
    static std::map<QString, CommandName> commandMap;
};
