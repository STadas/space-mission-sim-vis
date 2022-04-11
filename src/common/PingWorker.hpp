#pragma once

#include <QtCore>

/**
 * Ping worker class. Should be used in a separate thread. Regularly Emitted a
 * ping signal
 */
class PingWorker : public QObject
{
    Q_OBJECT

public:
    /**
     * Constructor
     */
    PingWorker();
    /**
     * Destructor
     */
    ~PingWorker();

    /**
     * Stop pinging
     */
    void stop();

signals:
    /**
     * @see PingWorker::onStart
     */
    void start();

    /**
     * Emitted regularly
     */
    void pinged();

private slots:
    /**
     * Starts pinging
     * @see PingWorker::start
     */
    void onStart();

private:
    std::atomic<bool> isActive_{};
};
