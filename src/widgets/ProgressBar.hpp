#pragma once

#include <QtCore>
#include <QtWidgets>

class ProgressBar : public QSlider
{
    Q_OBJECT

public:
    ProgressBar(QWidget *parent);
    ~ProgressBar();
};
