#include "MessageController.hpp"

MessageController::MessageController(QObject *parent)
    : QObject(parent)
{
    connect(this, qOverload<CommandErr, QWidget *>(&MessageController::error),
            this,
            qOverload<CommandErr, QWidget *>(&MessageController::onError));

    connect(
        this, qOverload<ConnectionErr, QWidget *>(&MessageController::error),
        this, qOverload<ConnectionErr, QWidget *>(&MessageController::onError));

    connect(this, qOverload<FileErr, QWidget *>(&MessageController::error),
            this, qOverload<FileErr, QWidget *>(&MessageController::onError));
}

MessageController::~MessageController()
{
}

void MessageController::onError(CommandErr err, QWidget *parent)
{
    QString windowTitle = "Syntax error";
    switch (err)
    {
        case CommandErr::NOT_IMPLEMENTED: {
            QMessageBox::critical(
                parent, windowTitle,
                "No such command implemented. Please check your "
                "syntax and try again.");
            break;
        }
        case CommandErr::BAD_ARG_COUNT: {
            QMessageBox::critical(
                parent, windowTitle,
                "Invalid amount of arguments for command. Please "
                "check your syntax and try again.");
            break;
        }
        case CommandErr::BAD_ARG_TYPE: {
            QMessageBox::critical(
                parent, windowTitle,
                "Invalid argument type for command. Please check"
                "your syntax and try again.");
            break;
        }
        case CommandErr::UNKNOWN: {
            QMessageBox::critical(parent, windowTitle,
                                  "Unknown syntax error. Please check your "
                                  "settings and syntax and try again.");
            break;
        }
        case CommandErr::EMPTY: {
            break;
        }
        case CommandErr::OK: {
            break;
        }
        default: {
            break;
        }
    }
}

void MessageController::onError(ConnectionErr err, QWidget *parent)
{
    QString windowTitle = "Connection error";
    switch (err)
    {
        case ConnectionErr::OK: {
            break;
        }
        case ConnectionErr::BAD_DATA: {
            QMessageBox::critical(
                parent, windowTitle,
                "There was an error when getting the preview image. Please "
                "check "
                "your settings and the status of the PANGU server.");
            break;
        }
        default: {
            QMessageBox::critical(
                parent, windowTitle,
                "There was an error when sending the command to the server. "
                "Please check your settings and the status of the server.");
            break;
        }
    }
}

void MessageController::onError(FileErr err, QWidget *parent)
{
    switch (err)
    {
        case FileErr::OPEN_FAIL: {
            QMessageBox::critical(parent, "Open file",
                                  "There was an error opening the file.");
            break;
        }
        case FileErr::SAVE_FAIL: {
            QMessageBox::critical(parent, "Save file as",
                                  "There was an error saving the file.");
            break;
        }
        default: {
        }
    }
}

QMessageBox::StandardButton MessageController::question(FileQuestion qst,
                                                        QWidget *parent)
{
    switch (qst)
    {
        case FileQuestion::FILE_NEW: {
            return QMessageBox::question(
                parent, "New file",
                "Are you sure? This will discard any unsaved changes.");
        }
        case FileQuestion::FILE_OPEN: {
            return QMessageBox::question(
                parent, "Open file",
                "Are you sure? This will discard any unsaved changes.");
        }
        default: {
            return QMessageBox::StandardButton::Close;
        }
    }
}
