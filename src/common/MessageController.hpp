#pragma once

#include <QtConcurrent>
#include <QtCore>
#include <QtWidgets>
#include "enums/CommandErr.hpp"
#include "enums/ConnectionErr.hpp"
#include "enums/FileErr.hpp"
#include "enums/FileQuestion.hpp"

/**
 * Message controller class. Used to show alert messages and other dialogues
 */
class MessageController : public QObject
{
    Q_OBJECT

public:
    /**
     * Constructor
     * @param parent Parent object
     */
    MessageController(QObject *parent);
    /**
     * Destructor
     */
    ~MessageController();

    /**
     * Show a file-related question dialog
     * @return Answer to the dialog
     */
    QMessageBox::StandardButton question(FileQuestion qst, QWidget *parent);

signals:
    /**
     * @see MessageController::onError(CommandErr, QWidget *)
     */
    void error(CommandErr err, QWidget *parent = nullptr);
    /**
     * @see MessageController::onError(ConnectionErr, QWidget *)
     */
    void error(ConnectionErr err, QWidget *parent = nullptr);
    /**
     * @see MessageController::onError(FileErr, QWidget *)
     */
    void error(FileErr err, QWidget *parent = nullptr);
    /**
     * @see MessageController::onError(QProcess::ProcessError, QWidget *)
     */
    void error(QProcess::ProcessError err, QWidget *parent = nullptr);

public slots:
    /**
     * Show a command-related error dialog
     * @param err Error status
     * @param parent The widget to show the dialog on top of
     * @see MessageController::error(CommandErr, QWidget *)
     */
    void onError(CommandErr err, QWidget *parent = nullptr);
    /**
     * Show a connection-related error dialog
     * @param err Error status
     * @param parent The widget to show the dialog on top of
     * @see MessageController::error(ConnectionErr, QWidget *)
     */
    void onError(ConnectionErr err, QWidget *parent = nullptr);
    /**
     * Show a file-related error dialog
     * @param err Error status
     * @param parent The widget to show the dialog on top of
     * @see MessageController::error(FileErr, QWidget *)
     */
    void onError(FileErr err, QWidget *parent = nullptr);
    /**
     * Show a server-related error dialog
     * @param err Error status
     * @param parent The widget to show the dialog on top of
     * @see MessageController::error(QProcess::ProcessError, QWidget *)
     */
    void onError(QProcess::ProcessError err, QWidget *parent = nullptr);

private:
    /**
     * Show error dialog but only if one isn't already open
     * @param parent The widget to show the dialog on top of
     * @param title Title of the dialog window
     * @param text Text of the dialog window
     */
    void showCritical(QWidget *parent, const QString &title,
                      const QString &text);

    bool messageOpen_;
};
