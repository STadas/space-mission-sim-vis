#include <gtest/gtest.h>

#include "common/PanguParser.hpp"

class PanguParserTest : public ::testing::Test
{
protected:
    PanguParserTest()
        : parser()
    {
    }

    PanguParser parser;
};

TEST_F(PanguParserTest, ParsePanguEmptyString)
{
    const QString str = "";

    auto parsed = parser.parse(str);

    ASSERT_EQ(parsed.err, CommandErr::EMPTY);
    ASSERT_FALSE(parsed.command.has_value());
}

TEST_F(PanguParserTest, ParsePanguComment)
{
    const QString str = "# foo bar";

    auto parsed = parser.parse(str);

    ASSERT_EQ(parsed.err, CommandErr::EMPTY);
    ASSERT_FALSE(parsed.command.has_value());
}

TEST_F(PanguParserTest, ParsePanguNotImplementedCommand)
{
    const QString str = "foo";

    auto parsed = parser.parse(str);

    ASSERT_EQ(parsed.err, CommandErr::NOT_IMPLEMENTED);
    ASSERT_FALSE(parsed.command.has_value());
}

TEST_F(PanguParserTest, ParsePanguStartNotEnoughArgs)
{
    const QString str = "start foo";

    auto parsed = parser.parse(str);

    ASSERT_EQ(parsed.err, CommandErr::BAD_ARG_COUNT);
    ASSERT_FALSE(parsed.command.has_value());
}

TEST_F(PanguParserTest, ParsePanguStartTooManyArgs)
{
    const QString str = "start 1 2 3 4 5 6 7";

    auto parsed = parser.parse(str);

    ASSERT_EQ(parsed.err, CommandErr::BAD_ARG_COUNT);
    ASSERT_FALSE(parsed.command.has_value());
}

TEST_F(PanguParserTest, ParsePanguStartBadArgType)
{
    const QString str = "start 1 2 3 4 5 foo";

    auto parsed = parser.parse(str);

    ASSERT_EQ(parsed.err, CommandErr::BAD_ARG_TYPE);
    ASSERT_FALSE(parsed.command.has_value());
}

TEST_F(PanguParserTest, ParsePanguStartOK)
{
    const QString str = "start 1 2 3 4 5 6";

    auto parsed = parser.parse(str);

    ASSERT_EQ(parsed.err, CommandErr::OK);
    ASSERT_TRUE(parsed.command.has_value());
}

TEST_F(PanguParserTest, ParsePanguOKWithComment)
{
    const QString str = "start 1 2 3 4 5 6 # foo bar";

    auto parsed = parser.parse(str);

    ASSERT_EQ(parsed.err, CommandErr::OK);
    ASSERT_TRUE(parsed.command.has_value());
}

TEST_F(PanguParserTest, ParsePanguOKWithCommentNoSpaceBetween)
{
    const QString str = "start 1 2 3 4 5 6# foo bar";

    auto parsed = parser.parse(str);

    ASSERT_EQ(parsed.err, CommandErr::OK);
    ASSERT_TRUE(parsed.command.has_value());
}

TEST_F(PanguParserTest, ParsePanguStartTrailingSpaces)
{
    const QString str = "start 1 2 3 4 5 6   ";

    auto parsed = parser.parse(str);

    ASSERT_EQ(parsed.err, CommandErr::OK);
    ASSERT_TRUE(parsed.command.has_value());
}

TEST_F(PanguParserTest, PanguStartTabs)
{
    const QString str = "start\t1\t2\t3\t4\t5\t6";

    auto parsed = parser.parse(str);

    ASSERT_EQ(parsed.err, CommandErr::OK);
    ASSERT_TRUE(parsed.command.has_value());
}

TEST_F(PanguParserTest, PanguStartMultipleSpacesAndTabs)
{
    const QString str = "start     1\t2\t      3\t4\t\t\t5\t6";

    auto parsed = parser.parse(str);

    ASSERT_EQ(parsed.err, CommandErr::OK);
    ASSERT_TRUE(parsed.command.has_value());
}

TEST_F(PanguParserTest, ParsePanguStartDouble)
{
    const QString str = "start 1 2 3 4.012938102471098123213098213 5 6";

    auto parsed = parser.parse(str);

    ASSERT_EQ(parsed.err, CommandErr::OK);
    ASSERT_TRUE(parsed.command.has_value());
}

TEST_F(PanguParserTest, ParsePanguQuaternionNotEnoughArgs)
{
    const QString str = "quaternion foo";

    auto parsed = parser.parse(str);

    ASSERT_EQ(parsed.err, CommandErr::BAD_ARG_COUNT);
    ASSERT_FALSE(parsed.command.has_value());
}

TEST_F(PanguParserTest, ParsePanguQuaternionTooManyArgs)
{
    const QString str = "quaternion 1 2 3 4 5 6 7 8";

    auto parsed = parser.parse(str);

    ASSERT_EQ(parsed.err, CommandErr::BAD_ARG_COUNT);
    ASSERT_FALSE(parsed.command.has_value());
}

TEST_F(PanguParserTest, ParsePanguQuaternionBadArgType)
{
    const QString str = "quaternion 1 2 3 4 5 6 foo";

    auto parsed = parser.parse(str);

    ASSERT_EQ(parsed.err, CommandErr::BAD_ARG_TYPE);
    ASSERT_FALSE(parsed.command.has_value());
}

TEST_F(PanguParserTest, ParsePanguQuaternionOK)
{
    const QString str = "quaternion 1 2 3 4 5 6 7";

    auto parsed = parser.parse(str);

    ASSERT_EQ(parsed.err, CommandErr::OK);
    ASSERT_TRUE(parsed.command.has_value());
}

TEST_F(PanguParserTest, ParsePanguQuaternionDouble)
{
    const QString str = "quaternion 1 2 3 4.012938102471098123213098213 5 6 7";

    auto parsed = parser.parse(str);

    ASSERT_EQ(parsed.err, CommandErr::OK);
    ASSERT_TRUE(parsed.command.has_value());
}

TEST_F(PanguParserTest, ParsePanguUpdateTooManyArgs)
{
    const QString str = "update foo";

    auto parsed = parser.parse(str);

    ASSERT_EQ(parsed.err, CommandErr::BAD_ARG_COUNT);
    ASSERT_FALSE(parsed.command.has_value());
}

TEST_F(PanguParserTest, ParsePanguUpdateOK)
{
    const QString str = "update";

    auto parsed = parser.parse(str);

    ASSERT_EQ(parsed.err, CommandErr::OK);
    ASSERT_TRUE(parsed.command.has_value());
}

TEST_F(PanguParserTest, ParsePanguPauseNotEnoughArgs)
{
    const QString str = "pause";

    auto parsed = parser.parse(str);

    ASSERT_EQ(parsed.err, CommandErr::BAD_ARG_COUNT);
    ASSERT_FALSE(parsed.command.has_value());
}

TEST_F(PanguParserTest, ParsePanguPauseTooManyArgs)
{
    const QString str = "pause 1 2";

    auto parsed = parser.parse(str);

    ASSERT_EQ(parsed.err, CommandErr::BAD_ARG_COUNT);
    ASSERT_FALSE(parsed.command.has_value());
}

TEST_F(PanguParserTest, ParsePanguPauseBadArgType)
{
    const QString str = "pause foo";

    auto parsed = parser.parse(str);

    ASSERT_EQ(parsed.err, CommandErr::BAD_ARG_TYPE);
    ASSERT_FALSE(parsed.command.has_value());
}

TEST_F(PanguParserTest, ParsePanguPauseOK)
{
    const QString str = "pause 1";

    auto parsed = parser.parse(str);

    ASSERT_EQ(parsed.err, CommandErr::OK);
    ASSERT_TRUE(parsed.command.has_value());
}

TEST_F(PanguParserTest, ParsePanguPauseDouble)
{
    const QString str = "pause 1.2218379264329384692384729347";

    auto parsed = parser.parse(str);

    ASSERT_EQ(parsed.err, CommandErr::OK);
    ASSERT_TRUE(parsed.command.has_value());
}
