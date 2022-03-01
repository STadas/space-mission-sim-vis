#pragma once

#include <QtConcurrent>
#include <QtCore>
#include <QtWidgets>

#include "Editor.hpp"
#include "common/MessageController.hpp"
#include "common/PreviewWorker.hpp"
#include "common/VBoxLayout.hpp"
#include "widgets/CamPreview.hpp"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Editor *editor_;
    CamPreview *preview_;
    ProgressBar *progressBar_;

    bool autoCommScan_;

    MessageController *messageController_;

    PreviewWorker *previewWorker_;

    QThread *previewWorkerThread_;

    QMenu *fileMenu_;
    QMenu *toolsMenu_;

    QAction *actFileNew_;
    QAction *actFileOpen_;
    QAction *actFileSave_;
    QAction *actFileSaveAs_;

    QAction *actActiveLineExec_;
    QAction *actMultiLineStart_;
    QAction *actMultiLineStop_;

    QAction *actCommScan;
    ;
    QAction *actToggleAutoCommScan_;

    void createMenus();
    void createActions();
    void createSignalConnections();

private slots:
    void newFile();
    void openFile();
    void saveFileAs();
    void saveFile();
};
