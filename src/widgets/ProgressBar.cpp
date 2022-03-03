#include "ProgressBar.hpp"

ProgressBar::ProgressBar(QWidget *parent)
    : QSlider(Qt::Horizontal, parent)
{
    this->setSingleStep(1);
    this->setPageStep(1);

    this->setTickInterval(1);
    this->setTickPosition(QSlider::TicksBothSides);

    this->setTracking(true);
    this->setMaximum(0);
    this->setDisabled(true);
}

ProgressBar::~ProgressBar()
{
}
