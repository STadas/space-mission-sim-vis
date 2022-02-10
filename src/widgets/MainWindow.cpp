#include "MainWindow.hpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->menuBar_ = new QMenuBar(this);
    this->centralWidget_ = new QWidget(this);
    this->statusBar_ = new QStatusBar(this);

    this->setCentralWidget(this->centralWidget_);
    this->setStatusBar(this->statusBar_);
    this->setMenuBar(this->menuBar_);

    this->centralLayout_ = new VBoxLayout(this->centralWidget_);
    this->textEdit_ = new QTextEdit(this->centralWidget_);

    this->centralLayout_->addWidget(this->textEdit_);
}
