#pragma once

#include <QtWidgets>

/**
 * Horizontal box layout class. Convenience class to change some defaults
 */
class HBoxLayout : public QHBoxLayout
{
    Q_OBJECT

public:
    /**
     * Constructor
     * @param parent Parent widget
     */
    HBoxLayout(QWidget *parent = nullptr);
};
