#pragma once

#include <QtCore>

class PingWorker : public QObject
{
    Q_OBJECT

public:
    PingWorker();
    ~PingWorker();

    void stop();

private:
    std::atomic<bool> isActive_{};

    void onStart();

signals:
    void start();

    void pinged();
};
