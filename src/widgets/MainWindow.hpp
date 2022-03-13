#pragma once

#include <QtConcurrent>
#include <QtCore>
#include <QtWidgets>

#include "Editor.hpp"
#include "common/MessageController.hpp"
#include "common/PanguServerProcess.hpp"
#include "common/PreviewWorker.hpp"
#include "common/VBoxLayout.hpp"
#include "common/settings/Settings.hpp"
#include "widgets/CamPreview.hpp"
#include "widgets/dialogs/SettingsDialog.hpp"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Editor *editor_;
    CamPreview *camPreview_;
    ProgressBar *progressBar_;

    bool autoCommScan_;

    MessageController *messageController_;

    PreviewWorker *previewWorker_;
    QThread *previewWorkerThread_;

    PanguServerProcess *serverProcess_;

    Settings *const settings_;

    QMenu *fileMenu_;
    QMenu *toolsMenu_;
    QMenu *commandsMenu_;
    QMenu *serverMenu_;

    QAction *actFileNew_;
    QAction *actFileOpen_;
    QAction *actFileSave_;
    QAction *actFileSaveAs_;

    QAction *actActiveLineExec_;
    QAction *actMultiLineStart_;
    QAction *actMultiLineStop_;

    QAction *actCommScan_;
    QAction *actToggleAutoCommScan_;

    QAction *actStartServer_;
    QAction *actStopServer_;
    QAction *actConnectToServer_;
    QAction *actDisconnectFromServer_;

    QAction *actOpenSettings_;

    void createMenus();
    void createActions();
    void createSignalConnections();

private slots:
    /* on action */
    void onActFileNew();
    void onActFileOpen();
    void onActFileSave();
    void onActFileSaveAs();

    void onActActiveLineExec();
    void onActMultiLineStart();
    void onActMultiLineStop();

    void onActCommScan();
    void onActToggleAutoCommScan();

    void onActStartServer();
    void onActStopServer();
    void onActConnectToServer();
    void onActDisconnectFromServer();

    void onActOpenSettings();

    /* on other signal */
    void onLineStarted(const int &lineNum);
    void onCommandError(CommandErr err);
    void onConnectionError(ConnectionErr err);

    void onMultiLineDone();
    void onChangePreview(QByteArray data, const unsigned long &size);

    void onPBarChanged(int imgIndex);
    void onPBarReleased();
    void onImgIndicesUpdated();

    void onEditorContentChanged();
};
