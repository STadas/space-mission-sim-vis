#pragma once

#include <QtCore>

#include "common/settings/Settings.hpp"
#include "interfaces/IProcess.hpp"

/**
 * PANGU server process class. Controls and communicates with the server
 * subprocess
 */
class PanguServerProcess : public QObject, public IProcess
{
    Q_OBJECT

public:
    /**
     * Constructor
     * @param parent Parent object
     * @param settings Settings object
     */
    PanguServerProcess(QObject *parent, Settings *const settings);
    /**
     * Destructor
     */
    ~PanguServerProcess() override;

    /**
     * Start the server
     * @param pathStr Path to the executable
     * @param args Argument list fo the executable
     */
    void start(const QString &pathStr, const QStringList &args = {}) override;
    /**
     * Stop the server
     */
    void stop() override;

signals:
    /**
     * Emitted when output is read from the subprocess
     * @param data Output from the subprocess
     */
    void output(QString data);
    /**
     * Emitted when there is an error in the subprocess
     * @param err Error from the subprocess
     */
    void error(QProcess::ProcessError err);

private slots:
    /**
     * Read standard output from the subprocess
     */
    void onReadyReadStdOut();
    /**
     * Read standard error output from the subprocess
     */
    void onReadyReadStdErr();

private:
    QProcess *process_;
    Settings *const settings_;
};
