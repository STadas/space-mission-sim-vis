#pragma once

#include <QtCore>

#include "interfaces/IServerProcess.hpp"

class PanguServerProcess : public QObject, IServerProcess
{
    Q_OBJECT

public:
    PanguServerProcess(QObject *parent);
    ~PanguServerProcess();

    void start(const QString &pathStr, const QStringList &args = {}) override;
    void stop() override;

private:
    QProcess *process_;

signals:
    void output(QString data);

private slots:
    // TODO: make stderr and stdout separate
    void onReadyRead();
};
