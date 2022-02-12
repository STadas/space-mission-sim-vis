#pragma once

#include <QtCore>
#include <QtWidgets>

#include "common/VBoxLayout.hpp"
#include "Editor.hpp"

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

    Editor *activeEditor_;

    void createMenus();
    void createActions();

    QMenu *fileMenu_;

    QAction *actFileNew_;
    QAction *actFileOpen_;
    QAction *actFileSave_;
    QAction *actFileSaveAs_;

private slots:
    void newFile();
    void openFile();
    void saveFileAs();
    void saveFile();
};
