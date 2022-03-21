#include <gtest/gtest.h>

#include "util/StringUtil.hpp"

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
