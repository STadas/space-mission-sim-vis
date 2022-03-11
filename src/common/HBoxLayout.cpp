#include "HBoxLayout.hpp"

HBoxLayout::HBoxLayout(QWidget *parent)
    : QHBoxLayout(parent)
{
    this->setSpacing(0);
    this->setMargin(0);
}
