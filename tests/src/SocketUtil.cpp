#include <gtest/gtest.h>

#include "util/SocketUtil.hpp"

TEST(SocketUtilTest, HostNameToAddrEmpty)
{
    QString s{};

    unsigned long res = SocketUtil::hostNameToAddr(s);

    ASSERT_EQ(res, INADDR_NONE);
}

TEST(SocketUtilTest, HostNameToAddrText)
{
    QString s = "foo bar";

    unsigned long res = SocketUtil::hostNameToAddr(s);

    ASSERT_EQ(res, INADDR_NONE);
}

TEST(SocketUtilTest, HostNameToAddr127001)
{
    QString s = "127.0.0.1";

    unsigned long res = SocketUtil::hostNameToAddr(s);

    ASSERT_EQ(res, inet_addr("127.0.0.1"));
}

TEST(SocketUtilTest, HostNameToAddrLocalhost)
{
    QString s = "localhost";

    unsigned long res = SocketUtil::hostNameToAddr(s);

    ASSERT_EQ(res, inet_addr("127.0.0.1"));
}

TEST(SocketUtilTest, HostNameToAddr8888)
{
    QString s = "8.8.8.8";

    unsigned long res = SocketUtil::hostNameToAddr(s);

    ASSERT_EQ(res, inet_addr("8.8.8.8"));
}
