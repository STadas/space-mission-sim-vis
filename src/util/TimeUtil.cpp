#include "TimeUtil.hpp"

void TimeUtil::delay(int msecs)
{
    /* https://stackoverflow.com/a/11487434 */
    QTime dieTime = QTime::currentTime().addMSecs(msecs);
    while (QTime::currentTime() < dieTime)
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
}
