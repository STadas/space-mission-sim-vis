#include "VBoxLayout.hpp"

VBoxLayout::VBoxLayout(QWidget *parent)
    : QVBoxLayout(parent)
{
    this->setSpacing(0);
    this->setMargin(0);
}
