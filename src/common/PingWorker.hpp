#pragma once

#include <QtCore>

class PingWorker : public QObject
{
    Q_OBJECT

public:
    PingWorker();
    ~PingWorker();

    void stop();

signals:
    void start();

    void pinged();

private slots:
    void onStart();

private:
    std::atomic<bool> isActive_{};
};
