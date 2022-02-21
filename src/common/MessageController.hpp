#pragma once

#include <QtCore>
#include <QtWidgets>
#include "enums/CommandErr.hpp"
#include "enums/ConnectionErr.hpp"
#include "enums/FileMessage.hpp"

class MessageController
{
public:
    static void commandError(CommandErr err, QWidget *parent);
    static void connectionError(ConnectionErr err, QWidget *parent);
    static QMessageBox::StandardButton fileMessage(FileMessage msg,
                                                   QWidget *parent);

private:
    MessageController(){};
};
