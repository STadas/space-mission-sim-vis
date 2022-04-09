#include "PanguServerProcess.hpp"

PanguServerProcess::PanguServerProcess(QObject *parent,
                                       Settings *const settings)
    : QObject(parent)
    , process_(new QProcess(this))
    , settings_(settings)
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
    QStringList settingsArgs{};

    if (this->settings_->serverCamPredefined.value())
    {
        settingsArgs += {
            "-use_save_size",
            "-save_width",
            QString::number(this->settings_->serverCamWidth.value()),
            "-save_height",
            QString::number(this->settings_->serverCamHeight.value()),
        };
    }

    if (this->settings_->serverIni.value().length() != 0)
    {
        settingsArgs += {
            "-ini",
            this->settings_->serverIni.value(),
        };
    }

    this->process_->start(pathStr, settingsArgs + args);
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
