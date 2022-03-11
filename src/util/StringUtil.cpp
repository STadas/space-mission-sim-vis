#include "StringUtil.hpp"

QStringList StringUtil::split(const QString &str, const QString &regexStr)
{
    std::string stdStr = str.toStdString();
    QStringList res{};

    /* https://stackoverflow.com/a/16752826 */
    /* https://stackoverflow.com/a/42310788 */
    std::regex rgx(regexStr.toStdString());
    std::sregex_token_iterator strIt(stdStr.begin(), stdStr.end(), rgx, -1);
    std::sregex_token_iterator end;

    for (; strIt != end; strIt++)
    {
        res.append(QString::fromStdString(*strIt));
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
