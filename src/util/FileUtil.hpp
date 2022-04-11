#pragma once

#include <QFile>

/**
 * File utilities class
 */
class FileUtil
{
public:
    /**
     * Write data to file
     * @param filePath File path to write to
     * @param data Data to write
     * @return Error status
     */
    static QFileDevice::FileError writeFile(const QString &filePath,
                                            const QString &data);
    /**
     * Read data from file
     * @param filePath File path to read from
     * @param data Variable to write data to
     * @return Error status
     */
    static QFileDevice::FileError readFile(const QString &filePath,
                                           QString &data);

private:
    /**
     * Private constructor
     */
    FileUtil()
    {
    }
};
