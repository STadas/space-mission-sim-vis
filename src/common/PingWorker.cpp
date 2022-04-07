#include "PingWorker.hpp"

PingWorker::PingWorker()
    : QObject(nullptr)
    , isActive_(false)
{
    QObject::connect(this, &PingWorker::start, this, &PingWorker::onStart);
}

PingWorker::~PingWorker()
{
}

void PingWorker::onStart()
{
    this->isActive_ = true;
    while (this->isActive_)
    {
        emit this->pinged();
        QThread::currentThread()->msleep(3000);
    }
}

void PingWorker::stop()
{
    this->isActive_ = false;
}
