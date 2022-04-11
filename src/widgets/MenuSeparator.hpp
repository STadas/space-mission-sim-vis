#pragma once

#include <QtCore>
#include <QtWidgets>

/**
 * Menu separator class
 */
class MenuSeparator : public QAction
{
    Q_OBJECT

public:
    /**
     * Constructor
     * @param parent Parent widget
     */
    MenuSeparator(QWidget *parent);
    /**
     * Destructor
     */
    ~MenuSeparator();
};
