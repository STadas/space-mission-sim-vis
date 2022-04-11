#pragma once

#include <QtWidgets>

/**
 * Vertical box layout class. Convenience class to change some defaults
 */
class VBoxLayout : public QVBoxLayout
{
    Q_OBJECT

public:
    /**
     * Constructor
     * @param parent Parent widget
     */
    VBoxLayout(QWidget *parent = nullptr);
};
