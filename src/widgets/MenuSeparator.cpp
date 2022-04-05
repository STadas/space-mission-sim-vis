#include "MenuSeparator.hpp"

MenuSeparator::MenuSeparator(QWidget *parent)
    : QAction(parent)
{
    this->setSeparator(true);
}

MenuSeparator::~MenuSeparator()
{
}
