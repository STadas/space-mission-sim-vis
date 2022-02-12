#include "VBoxLayout.hpp"

VBoxLayout::VBoxLayout(QWidget *parent)
    : QVBoxLayout(parent)
{
    this->setContentsMargins(0, 0, 0, 0);
    this->setSpacing(0);
    this->setMargin(0);
}
