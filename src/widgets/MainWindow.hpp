#pragma once

#include <QtCore>
#include <QtWidgets>

#include "Editor.hpp"
#include "common/MessageController.hpp"
#include "common/PanguConnection.hpp"
#include "common/VBoxLayout.hpp"
#include "util/CommandUtil.hpp"
#include "widgets/SimPreview.hpp"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow(){};

private:
    QWidget *centralWidget_;
    QMenuBar *menuBar_;
    QStatusBar *statusBar_;

    QLayout *centralLayout_;

    Editor *activeEditor_;
    SimPreview *activePreview_;
    IConnection *activeConnection;

    void createMenus();
    void createActions();

    QMenu *fileMenu_;
    QMenu *toolsMenu_;

    QAction *actFileNew_;
    QAction *actFileOpen_;
    QAction *actFileSave_;
    QAction *actFileSaveAs_;
    QAction *actLineExec_;

private slots:
    void newFile();
    void openFile();
    void saveFileAs();
    void saveFile();

    void execActiveLine();
};
