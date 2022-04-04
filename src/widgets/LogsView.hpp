#pragma once

#include <QtCore>
#include <QtWidgets>

class LogsView : public QPlainTextEdit
{
    Q_OBJECT

public:
    LogsView(QWidget *parent);
    ~LogsView();

    void append(const QString &str);
};
