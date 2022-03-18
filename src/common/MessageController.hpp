#pragma once

#include <QtConcurrent>
#include <QtCore>
#include <QtWidgets>
#include "enums/CommandErr.hpp"
#include "enums/ConnectionErr.hpp"
#include "enums/FileErr.hpp"
#include "enums/FileQuestion.hpp"

class MessageController : public QObject
{
    Q_OBJECT

public:
    MessageController(QObject *parent);
    ~MessageController();

    QMessageBox::StandardButton question(FileQuestion qst, QWidget *parent);

private:
    bool messageOpen_;
    void showCritical(QWidget *parent, const QString &title, const QString &text);

signals:
    void error(CommandErr err, QWidget *parent = nullptr);
    void error(ConnectionErr err, QWidget *parent = nullptr);
    void error(FileErr err, QWidget *parent = nullptr);

public slots:
    void onError(CommandErr err, QWidget *parent = nullptr);
    void onError(ConnectionErr err, QWidget *parent = nullptr);
    void onError(FileErr err, QWidget *parent = nullptr);
};
