#pragma once

#include <QtCore>
#include <QtWidgets>

/**
 * Progress bar class
 */
class ProgressBar : public QSlider
{
    Q_OBJECT

public:
    /**
     * Constructor
     * @param parent Parent widget
     */
    ProgressBar(QWidget *parent);
    /**
     * Destructor
     */
    ~ProgressBar();
};
