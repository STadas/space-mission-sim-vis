#pragma once

#include <QtCore>
#include <QtWidgets>
#include "enums/CommandErr.hpp"
#include "enums/ConnectionErr.hpp"
#include "enums/FileMessage.hpp"

class MessageController
{
public:
    static void error(CommandErr err, QWidget *parent);
    static void error(ConnectionErr err, QWidget *parent);

    static QMessageBox::StandardButton message(FileMessage msg,
                                               QWidget *parent);

private:
    MessageController(){};
};
