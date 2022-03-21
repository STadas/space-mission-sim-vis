#pragma once

#include <QtCore>
#include <regex>
#include <sstream>

class StringUtil
{
public:
    static bool isNumeric(const QString &str);

private:
    StringUtil()
    {
    }
};
