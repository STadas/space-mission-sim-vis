#pragma once

#include <QtCore>

#include "interfaces/IParser.hpp"
#include "util/StringUtil.hpp"

/**
 * PANGU command parser class
 */
class PanguParser : public QObject, public IParser
{
    Q_OBJECT

public:
    /**
     * Constructor
     * @param parent Parent object
     */
    PanguParser(QObject *parent = nullptr);
    /**
     * Destructor
     */
    ~PanguParser();

    /**
     * Parse a command
     * @param strCommand String to parse
     * @return The parsed result
     */
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
