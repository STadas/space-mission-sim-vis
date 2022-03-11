#include "PanguServerProcess.hpp"

PanguServerProcess::PanguServerProcess(QObject *parent)
    : QObject(parent)
    , process_(new QProcess(this))
{
    QObject::connect(this->process_, &QProcess::readyRead, this,
                     &PanguServerProcess::onReadyRead);
}

PanguServerProcess::~PanguServerProcess()
{
    this->stop();
}

void PanguServerProcess::start(const QString &pathStr, const QStringList &args)
{
    this->process_->start(pathStr, args);
}

void PanguServerProcess::stop()
{
    this->process_->close();
}

void PanguServerProcess::onReadyRead()
{
    emit this->output(this->process_->readAll());
}
