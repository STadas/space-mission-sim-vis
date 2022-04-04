#include "PanguServerProcess.hpp"

PanguServerProcess::PanguServerProcess(QObject *parent)
    : QObject(parent)
    , process_(new QProcess(this))
{
    QObject::connect(this->process_, &QProcess::readyReadStandardOutput, this,
                     &PanguServerProcess::onReadyReadStdOut);
    QObject::connect(this->process_, &QProcess::readyReadStandardError, this,
                     &PanguServerProcess::onReadyReadStdErr);
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

void PanguServerProcess::onReadyReadStdOut()
{
    QString out = this->process_->readAllStandardOutput().trimmed();
    emit this->output(out);
}

void PanguServerProcess::onReadyReadStdErr()
{
    QString err = this->process_->readAllStandardError().trimmed();
    emit this->output(err);
}
