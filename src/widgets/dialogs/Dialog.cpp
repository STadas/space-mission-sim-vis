#include "Dialog.hpp"

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
{
}

Dialog::~Dialog()
{
}

void Dialog::closeEvent(QCloseEvent *event)
{
    this->save();
    QDialog::closeEvent(event);
}
