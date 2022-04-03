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
            this->showCritical(
                parent, windowTitle,
                "Invalid amount of arguments for command. Please "
                "check your syntax and try again.");
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
        case ConnectionErr::Ok: {
            break;
        }
        case ConnectionErr::BadData: {
            this->showCritical(
                parent, windowTitle,
                "There was an error when getting the preview image. Please "
                "check "
                "your settings and the status of the PANGU server.");
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
            this->showCritical(parent, "Open file",
                               "There was an error opening the file.");
            break;
        }
        case FileErr::SaveFail: {
            this->showCritical(parent, "Save file as",
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
