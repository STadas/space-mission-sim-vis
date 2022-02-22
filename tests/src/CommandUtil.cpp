#include <gtest/gtest.h>

#include "util/CommandUtil.hpp"

TEST(CommandUtilTest, ParsePanguEmptyString)
{
    const QString str = "";
    std::unique_ptr<ParsedCommand> parsedCommand;

    auto err = CommandUtil::parsePangu(str, parsedCommand);

    ASSERT_EQ(err, CommandErr::EMPTY);
    ASSERT_EQ(parsedCommand, nullptr);
}

TEST(CommandUtilTest, ParsePanguNotImplementedCommand)
{
    const QString str = "foo";
    std::unique_ptr<ParsedCommand> parsedCommand;

    auto err = CommandUtil::parsePangu(str, parsedCommand);

    ASSERT_EQ(err, CommandErr::NOT_IMPLEMENTED);
    ASSERT_EQ(parsedCommand, nullptr);
}

TEST(CommandUtilTest, ParsePanguStartNotEnoughArgs)
{
    const QString str = "start foo";
    std::unique_ptr<ParsedCommand> parsedCommand;

    auto err = CommandUtil::parsePangu(str, parsedCommand);

    ASSERT_EQ(err, CommandErr::BAD_ARG_COUNT);
    ASSERT_EQ(parsedCommand, nullptr);
}

TEST(CommandUtilTest, ParsePanguStartTooManyArgs)
{
    const QString str = "start 1 2 3 4 5 6 7";
    std::unique_ptr<ParsedCommand> parsedCommand;

    auto err = CommandUtil::parsePangu(str, parsedCommand);

    ASSERT_EQ(err, CommandErr::BAD_ARG_COUNT);
    ASSERT_EQ(parsedCommand, nullptr);
}

TEST(CommandUtilTest, ParsePanguStartBadArgType)
{
    const QString str = "start 1 2 3 4 5 foo";
    std::unique_ptr<ParsedCommand> parsedCommand;

    auto err = CommandUtil::parsePangu(str, parsedCommand);

    ASSERT_EQ(err, CommandErr::BAD_ARG_TYPE);
    ASSERT_EQ(parsedCommand, nullptr);
}

TEST(CommandUtilTest, ParsePanguStartOK)
{
    const QString str = "start 1 2 3 4 5 6";
    std::unique_ptr<ParsedCommand> parsedCommand;

    auto err = CommandUtil::parsePangu(str, parsedCommand);

    ASSERT_EQ(err, CommandErr::OK);
    ASSERT_NE(parsedCommand, nullptr);
}

TEST(CommandUtilTest, ParsePanguStartDouble)
{
    const QString str = "start 1 2 3 4.012938102471098123213098213 5 6";
    std::unique_ptr<ParsedCommand> parsedCommand;

    auto err = CommandUtil::parsePangu(str, parsedCommand);

    ASSERT_EQ(err, CommandErr::OK);
    ASSERT_NE(parsedCommand, nullptr);
}

TEST(CommandUtilTest, ParsePanguQuaternionNotEnoughArgs)
{
    const QString str = "quaternion foo";
    std::unique_ptr<ParsedCommand> parsedCommand;

    auto err = CommandUtil::parsePangu(str, parsedCommand);

    ASSERT_EQ(err, CommandErr::BAD_ARG_COUNT);
    ASSERT_EQ(parsedCommand, nullptr);
}

TEST(CommandUtilTest, ParsePanguQuaternionTooManyArgs)
{
    const QString str = "quaternion 1 2 3 4 5 6 7 8";
    std::unique_ptr<ParsedCommand> parsedCommand;

    auto err = CommandUtil::parsePangu(str, parsedCommand);

    ASSERT_EQ(err, CommandErr::BAD_ARG_COUNT);
    ASSERT_EQ(parsedCommand, nullptr);
}

TEST(CommandUtilTest, ParsePanguQuaternionBadArgType)
{
    const QString str = "quaternion 1 2 3 4 5 6 foo";
    std::unique_ptr<ParsedCommand> parsedCommand;

    auto err = CommandUtil::parsePangu(str, parsedCommand);

    ASSERT_EQ(err, CommandErr::BAD_ARG_TYPE);
    ASSERT_EQ(parsedCommand, nullptr);
}

TEST(CommandUtilTest, ParsePanguQuaternionOK)
{
    const QString str = "quaternion 1 2 3 4 5 6 7";
    std::unique_ptr<ParsedCommand> parsedCommand;

    auto err = CommandUtil::parsePangu(str, parsedCommand);

    ASSERT_EQ(err, CommandErr::OK);
    ASSERT_NE(parsedCommand, nullptr);
}

TEST(CommandUtilTest, ParsePanguQuaternionDouble)
{
    const QString str = "quaternion 1 2 3 4.012938102471098123213098213 5 6 7";
    std::unique_ptr<ParsedCommand> parsedCommand;

    auto err = CommandUtil::parsePangu(str, parsedCommand);

    ASSERT_EQ(err, CommandErr::OK);
    ASSERT_NE(parsedCommand, nullptr);
}
