#include "StringUtil.hpp"

bool StringUtil::isNumeric(const QString &str)
{
    /* https://stackoverflow.com/a/29169469 */
    std::string stdStr = str.toStdString();
    double result{};
    std::stringstream iss(stdStr);

    iss >> result;
    return !iss.fail() && iss.eof();
}
