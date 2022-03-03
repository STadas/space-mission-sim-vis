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
    std::unique_ptr<ParsedCommand> parsedCommand;

    auto err = parser.parse(str, parsedCommand);

    ASSERT_EQ(err, CommandErr::EMPTY);
    ASSERT_EQ(parsedCommand, nullptr);
}

TEST_F(PanguParserTest, ParsePanguNotImplementedCommand)
{
    const QString str = "foo";
    std::unique_ptr<ParsedCommand> parsedCommand;

    auto err = parser.parse(str, parsedCommand);

    ASSERT_EQ(err, CommandErr::NOT_IMPLEMENTED);
    ASSERT_EQ(parsedCommand, nullptr);
}

TEST_F(PanguParserTest, ParsePanguStartNotEnoughArgs)
{
    const QString str = "start foo";
    std::unique_ptr<ParsedCommand> parsedCommand;

    auto err = parser.parse(str, parsedCommand);

    ASSERT_EQ(err, CommandErr::BAD_ARG_COUNT);
    ASSERT_EQ(parsedCommand, nullptr);
}

TEST_F(PanguParserTest, ParsePanguStartTooManyArgs)
{
    const QString str = "start 1 2 3 4 5 6 7";
    std::unique_ptr<ParsedCommand> parsedCommand;

    auto err = parser.parse(str, parsedCommand);

    ASSERT_EQ(err, CommandErr::BAD_ARG_COUNT);
    ASSERT_EQ(parsedCommand, nullptr);
}

TEST_F(PanguParserTest, ParsePanguStartBadArgType)
{
    const QString str = "start 1 2 3 4 5 foo";
    std::unique_ptr<ParsedCommand> parsedCommand;

    auto err = parser.parse(str, parsedCommand);

    ASSERT_EQ(err, CommandErr::BAD_ARG_TYPE);
    ASSERT_EQ(parsedCommand, nullptr);
}

TEST_F(PanguParserTest, ParsePanguStartOK)
{
    const QString str = "start 1 2 3 4 5 6";
    std::unique_ptr<ParsedCommand> parsedCommand;

    auto err = parser.parse(str, parsedCommand);

    ASSERT_EQ(err, CommandErr::OK);
    ASSERT_NE(parsedCommand, nullptr);
}

TEST_F(PanguParserTest, ParsePanguStartTrailingSpaces)
{
    const QString str = "start 1 2 3 4 5 6   ";
    std::unique_ptr<ParsedCommand> parsedCommand;

    auto err = parser.parse(str, parsedCommand);

    ASSERT_EQ(err, CommandErr::OK);
    ASSERT_NE(parsedCommand, nullptr);
}

TEST_F(PanguParserTest, PanguStartTabs)
{
    const QString str = "start\t1\t2\t3\t4\t5\t6";
    std::unique_ptr<ParsedCommand> parsedCommand;

    auto err = parser.parse(str, parsedCommand);

    ASSERT_EQ(err, CommandErr::OK);
    ASSERT_NE(parsedCommand, nullptr);
}

TEST_F(PanguParserTest, PanguStartMultipleSpacesAndTabs)
{
    const QString str = "start     1\t2\t      3\t4\t\t\t5\t6";
    std::unique_ptr<ParsedCommand> parsedCommand;

    auto err = parser.parse(str, parsedCommand);

    ASSERT_EQ(err, CommandErr::OK);
    ASSERT_NE(parsedCommand, nullptr);
}

TEST_F(PanguParserTest, ParsePanguStartDouble)
{
    const QString str = "start 1 2 3 4.012938102471098123213098213 5 6";
    std::unique_ptr<ParsedCommand> parsedCommand;

    auto err = parser.parse(str, parsedCommand);

    ASSERT_EQ(err, CommandErr::OK);
    ASSERT_NE(parsedCommand, nullptr);
}

TEST_F(PanguParserTest, ParsePanguQuaternionNotEnoughArgs)
{
    const QString str = "quaternion foo";
    std::unique_ptr<ParsedCommand> parsedCommand;

    auto err = parser.parse(str, parsedCommand);

    ASSERT_EQ(err, CommandErr::BAD_ARG_COUNT);
    ASSERT_EQ(parsedCommand, nullptr);
}

TEST_F(PanguParserTest, ParsePanguQuaternionTooManyArgs)
{
    const QString str = "quaternion 1 2 3 4 5 6 7 8";
    std::unique_ptr<ParsedCommand> parsedCommand;

    auto err = parser.parse(str, parsedCommand);

    ASSERT_EQ(err, CommandErr::BAD_ARG_COUNT);
    ASSERT_EQ(parsedCommand, nullptr);
}

TEST_F(PanguParserTest, ParsePanguQuaternionBadArgType)
{
    const QString str = "quaternion 1 2 3 4 5 6 foo";
    std::unique_ptr<ParsedCommand> parsedCommand;

    auto err = parser.parse(str, parsedCommand);

    ASSERT_EQ(err, CommandErr::BAD_ARG_TYPE);
    ASSERT_EQ(parsedCommand, nullptr);
}

TEST_F(PanguParserTest, ParsePanguQuaternionOK)
{
    const QString str = "quaternion 1 2 3 4 5 6 7";
    std::unique_ptr<ParsedCommand> parsedCommand;

    auto err = parser.parse(str, parsedCommand);

    ASSERT_EQ(err, CommandErr::OK);
    ASSERT_NE(parsedCommand, nullptr);
}

TEST_F(PanguParserTest, ParsePanguQuaternionDouble)
{
    const QString str = "quaternion 1 2 3 4.012938102471098123213098213 5 6 7";
    std::unique_ptr<ParsedCommand> parsedCommand;

    auto err = parser.parse(str, parsedCommand);

    ASSERT_EQ(err, CommandErr::OK);
    ASSERT_NE(parsedCommand, nullptr);
}
