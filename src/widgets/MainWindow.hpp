#pragma once

#include <QtCore>
#include <QtWidgets>

#include "VBoxLayout.hpp"
#include "util/FileUtil.hpp"

//TODO: move file stuff somewhere else?

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

    void createMenus();
    void createActions();

    QString defaultSavePath_{};
    QMenu *fileMenu_;

    QAction *actNewFile_;
    QAction *actOpenFile_;
    QAction *actSaveFile_;
    QAction *actSaveFileAs_;

private slots:
    void newFile();
    void openFile();
    void saveFileAs();
    void saveFile();
};
