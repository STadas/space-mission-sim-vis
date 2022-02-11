#pragma once

#include <QFile>
#include <optional>

namespace FileUtil
{
static QFileDevice::FileError writeFile(const QString &data, const QString &filePath)
{
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly))
    {
        file.write(data.toUtf8());
        file.close();
    }
    return file.error();
}

//TODO: error codes
static std::optional<QString> readFile(const QString &filePath)
{
    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly))
    {
        QString res = file.readAll();
        file.close();
        return res;
    }
    return {};
}
} // namespace FileUtil
