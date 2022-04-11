#pragma once

#include <QtCore>
#include <regex>
#include <sstream>

/**
 * String utilities class
 */
class StringUtil
{
public:
    /**
     * Check if string is numeric
     * @param str String to check
     * @return Whether the string is numeric or not
     */
    static bool isNumeric(const QString &str);

private:
    /**
     * Private constructor
     */
    StringUtil()
    {
    }
};
