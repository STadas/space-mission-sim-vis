#pragma once

#include <QtCore>

#include "common/settings/Settings.hpp"
#include "interfaces/IProcess.hpp"

class PanguServerProcess : public QObject, public IProcess
{
    Q_OBJECT

public:
    PanguServerProcess(QObject *parent, Settings *const settings);
    ~PanguServerProcess() override;

    void start(const QString &pathStr, const QStringList &args = {}) override;
    void stop() override;

private:
    QProcess *process_;
    Settings *const settings_;

signals:
    void output(QString data);

private slots:
    void onReadyReadStdOut();
    void onReadyReadStdErr();
};
