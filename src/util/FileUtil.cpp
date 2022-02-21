#include "FileUtil.hpp"

QFileDevice::FileError FileUtil::writeFile(const QString &filePath,
                                           const QString &data)
{
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly))
    {
        file.write(data.toUtf8());
        file.close();
    }
    return file.error();
}

QFileDevice::FileError FileUtil::readFile(const QString &filePath,
                                          QString &data)
{
    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly))
    {
        data = file.readAll();
        file.close();
    }
    return file.error();
}
