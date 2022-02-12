#pragma once

#include <QFile>

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

static QString readFile(const QString &filePath, int &err)
{
    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly))
    {
        QString res = file.readAll();
        file.close();
        err = file.error();
        return res;
    }
    err = file.error();
    return {};
}
} // namespace FileUtil
