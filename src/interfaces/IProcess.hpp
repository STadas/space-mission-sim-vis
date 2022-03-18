#pragma once

#include <QtCore>

class IProcess
{
public:
    ~IProcess()
    {
    }

    virtual void start(const QString &pathStr, const QStringList &args) = 0;
    virtual void stop() = 0;
};
