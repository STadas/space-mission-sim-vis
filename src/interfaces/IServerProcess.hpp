#pragma once

#include <QtCore>

class IServerProcess
{
public:
    ~IServerProcess()
    {
    }

    virtual void start(const QString &pathStr, const QStringList &args) = 0;
    virtual void stop() = 0;
};
