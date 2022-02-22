#include "StringUtil.hpp"

std::vector<QString> StringUtil::split(const QString &str,
                                       const char &separator)
{
    std::istringstream iss(str.toStdString());
    std::string word;
    std::vector<QString> res{};
    while (std::getline(iss, word, separator))
    {
        res.push_back(QString::fromStdString(word));
    }
    return res;
}

bool StringUtil::isNumeric(const QString &str)
{
    /* https://stackoverflow.com/a/29169469 */
    std::string stdStr = str.toStdString();
    double result{};
    std::stringstream iss(stdStr);

    iss >> result;

    return !iss.fail() && iss.eof();
}
