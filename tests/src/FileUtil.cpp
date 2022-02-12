#include <gtest/gtest.h>
#include <QString>
#include <fstream>
#include <sstream>

#include "util/FileUtil.hpp"

TEST(FileUtilTest, WriteExistingEmpty)
{
    const QString filePath = "files/notYetWrittenFile.txt";
    const QString writtenData = "foo bar\n";

    auto err = FileUtil::writeFile(filePath, writtenData);

    // Errcode
    ASSERT_EQ(err, QFileDevice::FileError::NoError);

    // File contents after write
    QString data{};
    std::ifstream ifs;
    ifs.open(filePath.toStdString());
    if (ifs.is_open())
    {
        std::stringstream ss;
        ss << ifs.rdbuf();
        ifs.close();

        data = QString::fromStdString(ss.str());
    }
    ASSERT_EQ(data, writtenData);
}

TEST(FileUtilTest, WriteExistingNonEmpty)
{
    const QString filePath = "files/nonEmptyNotYetWrittenFile.txt";
    const QString writtenData = "bar\n";

    auto err = FileUtil::writeFile(filePath, writtenData);

    // Errcode
    ASSERT_EQ(err, QFileDevice::FileError::NoError);

    // File contents after write
    QString data{};
    std::ifstream ifs;
    ifs.open(filePath.toStdString());
    if (ifs.is_open())
    {
        std::stringstream ss;
        ss << ifs.rdbuf();
        ifs.close();

        data = QString::fromStdString(ss.str());
    }
    ASSERT_EQ(data, writtenData);
}

TEST(FileUtilTest, WriteNonExisting)
{
    const QString filePath = "files/notYetExistingFile.txt";
    const QString writtenData = "foo bar\n";

    int err = FileUtil::writeFile(filePath, writtenData);

    // Errcode
    ASSERT_EQ(err, QFileDevice::FileError::NoError);

    // File contents after write
    QString data{};
    std::ifstream ifs;
    ifs.open(filePath.toStdString());
    if (ifs.is_open())
    {
        std::stringstream ss;
        ss << ifs.rdbuf();
        ifs.close();

        data = QString::fromStdString(ss.str());
    }
    ASSERT_EQ(data, writtenData);
}

TEST(FileUtilTest, ReadExisting)
{
    const QString filePath = "files/fileWithText.txt";
    const QString expectedData = "foo bar\n";

    QFileDevice::FileError err;
    QString data = FileUtil::readFile(filePath, err);

    ASSERT_EQ(err, QFileDevice::FileError::NoError);
    ASSERT_EQ(data, expectedData);
}

TEST(FileUtilTest, ReadExistingEmpty)
{
    const QString filePath = "files/emptyFile.txt";
    const QString expectedData = "";

    QFileDevice::FileError err;
    QString data = FileUtil::readFile(filePath, err);

    ASSERT_EQ(err, QFileDevice::FileError::NoError);
    ASSERT_EQ(data, expectedData);
}

TEST(FileUtilTest, ReadNonExisting)
{
    const QString filePath = "files/nonExistingFile.txt";

    QFileDevice::FileError err;
    QString data = FileUtil::readFile(filePath, err);

    ASSERT_EQ(err, QFileDevice::FileError::OpenError);
}
