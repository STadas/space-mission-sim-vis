#pragma once

#include <QtCore>
#include <QtWidgets>

class Action : public QAction
{
    Q_OBJECT

public:
    using QAction::QAction;
    void setStatusTip(const QString &statusTip);
};
