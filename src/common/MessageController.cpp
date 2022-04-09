#include "MessageController.hpp"

MessageController::MessageController(QObject *parent)
    : QObject(parent)
    , messageOpen_(false)
{
    QObject::connect(
        this, qOverload<CommandErr, QWidget *>(&MessageController::error), this,
        qOverload<CommandErr, QWidget *>(&MessageController::onError));

    QObject::connect(
        this, qOverload<ConnectionErr, QWidget *>(&MessageController::error),
        this, qOverload<ConnectionErr, QWidget *>(&MessageController::onError));

    QObject::connect(
        this, qOverload<FileErr, QWidget *>(&MessageController::error), this,
        qOverload<FileErr, QWidget *>(&MessageController::onError));

    using ProcessError = QProcess::ProcessError;
    QObject::connect(
        this, qOverload<ProcessError, QWidget *>(&MessageController::error),
        this, qOverload<ProcessError, QWidget *>(&MessageController::onError));
}

MessageController::~MessageController()
{
}

void MessageController::showCritical(QWidget *parent, const QString &title,
                                     const QString &text)
{
    if (messageOpen_)
        return;

    this->messageOpen_ = true;

    QMessageBox *mBox = new QMessageBox(parent);

    mBox->setModal(false);
    mBox->setIconPixmap(
        parent->style()->standardPixmap(QStyle::SP_MessageBoxCritical));
    mBox->setWindowTitle(title);
    mBox->setText(text);
    mBox->addButton(QMessageBox::Ok);
    QObject::connect(mBox, &QMessageBox::buttonClicked, this, [=] {
        this->messageOpen_ = false;
    });

    mBox->show();
}

void MessageController::onError(CommandErr err, QWidget *parent)
{
    // we don't want to spam this one in case the user quickly executes a bunch
    // of commands

    QString windowTitle = "Syntax error";
    switch (err)
    {
        case CommandErr::NotImplemented: {
            this->showCritical(parent, windowTitle,
                               "No such command implemented. Please check your "
                               "syntax and try again.");
            break;
        }
        case CommandErr::BadArgCount: {
            this->showCritical(parent, windowTitle,
                               "Invalid amount of arguments for command. "
                               "Please check your syntax and try again.");
            break;
        }
        case CommandErr::BadArgType: {
            this->showCritical(parent, windowTitle,
                               "Invalid argument type for command. Please check"
                               "your syntax and try again.");
            break;
        }
        case CommandErr::Unknown: {
            this->showCritical(parent, windowTitle,
                               "Unknown syntax error. Please check your "
                               "settings and syntax and try again.");
            break;
        }
        case CommandErr::Empty: {
            break;
        }
        case CommandErr::Ok: {
            break;
        }
    }
}

void MessageController::onError(ConnectionErr err, QWidget *parent)
{
    // we don't want to spam this one in case the user quickly executes a bunch
    // of commands

    QString windowTitle = "Connection error";
    switch (err)
    {
        case ConnectionErr::Ok: {
            break;
        }
        case ConnectionErr::BadData: {
            this->showCritical(
                parent, windowTitle,
                "There was an error when getting the preview image. Please "
                "check your settings and the status of the PANGU server.");
            break;
        }
        default: {
            this->showCritical(
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
        case FileErr::OpenFail: {
            QMessageBox::critical(parent, "Open file",
                                  "There was an error opening the file. Please "
                                  "check if it still exists and if you have "
                                  "permissions to open it.");
            break;
        }
        case FileErr::SaveFail: {
            QMessageBox::critical(
                parent, "Save file",
                "There was an error saving the file. Please check if you have "
                "permissions to write to it.");
            break;
        }
    }
}

QMessageBox::StandardButton MessageController::question(FileQuestion qst,
                                                        QWidget *parent)
{
    switch (qst)
    {
        case FileQuestion::FileNew: {
            return QMessageBox::question(
                parent, "New file",
                "Are you sure? This will discard any unsaved changes.");
        }
        case FileQuestion::FileOpen: {
            return QMessageBox::question(
                parent, "Open file",
                "Are you sure? This will discard any unsaved changes.");
        }
        default: {
            return QMessageBox::StandardButton::Close;
        }
    }
}

void MessageController::onError(QProcess::ProcessError err, QWidget *parent)
{
    using ProcessError = QProcess::ProcessError;

    QString windowTitle = "Server executable error";

    switch (err)
    {
        case ProcessError::FailedToStart: {
            QMessageBox::critical(
                parent, windowTitle,
                "The server failed to start. Please check the settings and the "
                "executable if you've configured them correctly.");
            break;
        }
        case ProcessError::Crashed: {
            QMessageBox::critical(
                parent, windowTitle,
                "The server has crashed. Please check the settings and the "
                "executable if you've configured them correctly.");
            break;
        }
        case ProcessError::Timedout: {
            QMessageBox::critical(
                parent, windowTitle,
                "The server has timed out. Please check the settings and the "
                "executable if you've configured them correctly.");
            break;
        }
        default: {
            QMessageBox::critical(
                parent, windowTitle,
                "The server has encountered an unexpected error. Please check "
                "the settings and the executable if you've configured them "
                "correctly.");
            break;
        }
    }
}
