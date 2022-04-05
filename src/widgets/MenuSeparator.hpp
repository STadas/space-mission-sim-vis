#pragma once

#include <QtCore>
#include <QtWidgets>

class MenuSeparator : public QAction
{
    Q_OBJECT

public:
    MenuSeparator(QWidget *parent);
    ~MenuSeparator();
};
