#pragma once

#include <QtCore>
#include <sstream>

class StringUtil
{
public:
    static std::vector<QString> split(const QString &str,
                                      const char &separator);
    static bool isNumeric(const QString &str);

private:
    StringUtil(){};
};
