#pragma once

#include <QtCore>

/**
 * Thread utilities class
 */
class ThreadUtil
{
public:
    /**
     * @return Whether the call is in the GUI thread
     */
    static bool isGuiThread();

private:
    /**
     * Private constructor
     */
    ThreadUtil()
    {
    }
};
