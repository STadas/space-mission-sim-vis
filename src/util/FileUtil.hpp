#pragma once

#include <QFile>

class FileUtil
{
public:
    static QFileDevice::FileError writeFile(const QString &filePath,
                                            const QString &data);
    static QFileDevice::FileError readFile(const QString &filePath,
                                           QString &data);

private:
    FileUtil()
    {
    }
};
