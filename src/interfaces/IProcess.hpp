#pragma once

#include <QtCore>

/**
 * Process interface
 */
class IProcess
{
public:
    /**
     * Virtual destructor
     */
    virtual ~IProcess()
    {
    }

    /**
     * Start the process
     * @param pathStr Path to the executable
     * @param args Argument list fo the executable
     */
    virtual void start(const QString &pathStr, const QStringList &args) = 0;
    /**
     * Stop the process
     */
    virtual void stop() = 0;
};
