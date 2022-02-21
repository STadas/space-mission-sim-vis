#include "SocketUtil.hpp"

unsigned long SocketUtil::hostNameToAddr(const QString &s)
{
    /* Modified example provided with PANGU */

    std::string stdStr = s.toStdString();
    struct hostent *host;

    /* Assume we have a dotted IP address ... */
    long result = inet_addr(stdStr.c_str());
    if (result != (long)INADDR_NONE)
        return result;

    /* That failed so assume DNS will resolve it. */
    host = gethostbyname(stdStr.c_str());
    return host ? *((long *)host->h_addr_list[0]) : INADDR_NONE;
}
