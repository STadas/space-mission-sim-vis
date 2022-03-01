#pragma once

#include <QtCore>
#include <regex>
#include <sstream>

class StringUtil
{
public:
    static std::vector<QString> split(const QString &str,
                                      const QString &regexStr = " ");
    static bool isNumeric(const QString &str);

private:
    StringUtil(){};
};
