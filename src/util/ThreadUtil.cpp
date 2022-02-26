#include "ThreadUtil.hpp"

bool ThreadUtil::isGuiThread()
{
    return QThread::currentThread() == QCoreApplication::instance()->thread();
}
