#pragma once

#include <QtConcurrent>
#include <QtCore>
#include <QtWidgets>

#include "PanguEditor.hpp"
#include "common/MessageController.hpp"
#include "common/PanguConnection.hpp"
#include "common/PanguWorker.hpp"
#include "common/VBoxLayout.hpp"
#include "util/CommandUtil.hpp"
#include "widgets/SimPreview.hpp"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    PanguWorker *panguWorker_;
    PanguEditor *editor_;
    SimPreview *preview_;
    MessageController *messageController_;

    QThread *workerThread_;

    QMenu *fileMenu_;
    QMenu *toolsMenu_;

    QAction *actFileNew_;
    QAction *actFileOpen_;
    QAction *actFileSave_;
    QAction *actFileSaveAs_;

    QAction *actActiveLineExec_;
    QAction *actMultiLineStart_;
    QAction *actMultiLineStop_;

    void createMenus();
    void createActions();
    void createSignalConnections();

private slots:
    void newFile();
    void openFile();
    void saveFileAs();
    void saveFile();
};
