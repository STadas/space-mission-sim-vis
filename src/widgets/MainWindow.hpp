#pragma once

#include <QtCore>
#include <QtWidgets>

#include "VBoxLayout.hpp"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private:
    QWidget *centralWidget_;
    QMenuBar *menuBar_;
    QStatusBar *statusBar_;

    QLayout *centralLayout_;

    QTextEdit *textEdit_;
};
