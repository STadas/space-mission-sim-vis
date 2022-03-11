#pragma once

#include <QtCore>

class ThreadUtil
{
public:
    static bool isGuiThread();

private:
    ThreadUtil()
    {
    }
};
