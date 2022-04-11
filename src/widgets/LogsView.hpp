#pragma once

#include <QtCore>
#include <QtWidgets>

/**
 * Logs view class
 */
class LogsView : public QPlainTextEdit
{
    Q_OBJECT

public:
    /**
     * Constructor
     * @param parent Parent widget
     */
    LogsView(QWidget *parent);
    /**
     * Destructor
     */
    ~LogsView();
};
