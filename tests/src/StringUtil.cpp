#include <gtest/gtest.h>

#include "util/StringUtil.hpp"

TEST(StringUtilTest, SplitSpaceEmpty)
{
    const QString str = "";

    std::vector<QString> res = StringUtil::split(str, ' ');

    ASSERT_EQ(res.size(), 0);
}

TEST(StringUtilTest, SplitSpaceOneSpace)
{
    const QString str = " ";

    std::vector<QString> res = StringUtil::split(str, ' ');

    ASSERT_EQ(res.size(), 1);
    ASSERT_EQ(res[0], "");
}

TEST(StringUtilTest, SplitSpaceTwoSpaces)
{
    const QString str = "  ";

    std::vector<QString> res = StringUtil::split(str, ' ');

    ASSERT_EQ(res.size(), 2);
    ASSERT_EQ(res[0], "");
    ASSERT_EQ(res[1], "");
}

TEST(StringUtilTest, SplitSpaceOneWord)
{
    const QString str = "foo";

    std::vector<QString> res = StringUtil::split(str, ' ');

    ASSERT_EQ(res.size(), 1);
    ASSERT_EQ(res[0], "foo");
}

TEST(StringUtilTest, SplitSpaceOneWordOneSpace)
{
    const QString str = "foo ";

    std::vector<QString> res = StringUtil::split(str, ' ');

    ASSERT_EQ(res.size(), 1);
    ASSERT_EQ(res[0], "foo");
}

TEST(StringUtilTest, SplitSpaceOneSpaceOneWord)
{
    const QString str = " foo";

    std::vector<QString> res = StringUtil::split(str, ' ');

    ASSERT_EQ(res.size(), 2);
    ASSERT_EQ(res[0], "");
    ASSERT_EQ(res[1], "foo");
}

TEST(StringUtilTest, SplitSeparatorTwoWords)
{
    const QString str = "foo bar";

    std::vector<QString> res = StringUtil::split(str, ' ');

    ASSERT_EQ(res.size(), 2);
    ASSERT_EQ(res[0], "foo");
    ASSERT_EQ(res[1], "bar");
}

TEST(StringUtilTest, SplitSeparatorFourWords)
{
    const QString str = "foo bar baz qux";

    std::vector<QString> res = StringUtil::split(str, ' ');

    ASSERT_EQ(res.size(), 4);
    ASSERT_EQ(res[0], "foo");
    ASSERT_EQ(res[1], "bar");
    ASSERT_EQ(res[2], "baz");
    ASSERT_EQ(res[3], "qux");
}

TEST(StringUtilTest, IsNumericLetters)
{
    const QString str = "foo bar";

    bool res = StringUtil::isNumeric(str);

    ASSERT_FALSE(res);
}

TEST(StringUtilTest, IsNumericPunctuation)
{
    const QString str = ".,~/!";

    bool res = StringUtil::isNumeric(str);

    ASSERT_FALSE(res);
}

TEST(StringUtilTest, IsNumericDot)
{
    const QString str = ".";

    bool res = StringUtil::isNumeric(str);

    ASSERT_FALSE(res);
}

TEST(StringUtilTest, IsNumericIntShort)
{
    const QString str = "8";

    bool res = StringUtil::isNumeric(str);

    ASSERT_TRUE(res);
}

TEST(StringUtilTest, IsNumericIntTooLongTruncated)
{
    const QString str = "89128319284719287301287591283701237192841231231231";

    bool res = StringUtil::isNumeric(str);

    ASSERT_TRUE(res);
}

TEST(StringUtilTest, IsNumericIntNegative)
{
    const QString str = "-1";

    bool res = StringUtil::isNumeric(str);

    ASSERT_TRUE(res);
}

TEST(StringUtilTest, IsNumericFloatingShort)
{
    const QString str = "0.123";

    bool res = StringUtil::isNumeric(str);

    ASSERT_TRUE(res);
}

TEST(StringUtilTest, IsNumericFloatingNothingBeforeDot)
{
    const QString str = ".123";

    bool res = StringUtil::isNumeric(str);

    ASSERT_TRUE(res);
}

TEST(StringUtilTest, IsNumericFloatingTooLongTruncated)
{
    const QString str = "0.123123098203947032849023947023957412302971209384";

    bool res = StringUtil::isNumeric(str);

    ASSERT_TRUE(res);
}

TEST(StringUtilTest, IsNumericFloatNegative)
{
    const QString str = "-0.123";

    bool res = StringUtil::isNumeric(str);

    ASSERT_TRUE(res);
}

TEST(StringUtilTest, IsNumericFloatingNothingBeforeDotNegative)
{
    const QString str = "-.123";

    bool res = StringUtil::isNumeric(str);

    ASSERT_TRUE(res);
}
