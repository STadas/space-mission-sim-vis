#pragma once

#include <QtConcurrent>
#include <QtCore>
#include <QtWidgets>

#include "common/MessageController.hpp"
#include "common/PanguServerProcess.hpp"
#include "common/PreviewWorker.hpp"
#include "common/Resources.hpp"
#include "common/VBoxLayout.hpp"
#include "common/settings/Settings.hpp"
#include "interfaces/ISavableLoadable.hpp"
#include "widgets/CamPreview.hpp"
#include "widgets/Editor.hpp"
#include "widgets/PlaybackInterface.hpp"
#include "widgets/dialogs/SettingsDialog.hpp"

class MainWindow : public QMainWindow, public ISavableLoadable
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void closeEvent(QCloseEvent *event) override;

private:
    void save() override;
    void load() override;

    void initSignalConnections();
    void initActions();
    void initMenus();
    void initToolBars();
    void initPlayBackInterface();

    Settings *const settings_;
    Resources *const resources_;

    Editor *editor_;
    CamPreview *camPreview_;
    PlaybackInterface *playbackInterface_;
    ProgressBar *progressBar_;

    QDockWidget *dockCamPreview_;
    QDockWidget *dockPlaybackInterface_;

    bool autoCommScan_;

    MessageController *messageController_;

    PreviewWorker *previewWorker_;
    QThread *previewWorkerThread_;

    PanguServerProcess *serverProcess_;

    QMenu *fileMenu_;
    QMenu *editMenu_;
    QMenu *viewMenu_;
    QMenu *toolsMenu_;
    QMenu *commandsMenu_;
    QMenu *serverMenu_;

    QAction *actNewFile_;
    QAction *actOpenFile_;
    QAction *actSaveFile_;
    QAction *actSaveFileAs_;

    QAction *actUndo_;
    QAction *actRedo_;
    QAction *actCut_;
    QAction *actCopy_;
    QAction *actPaste_;

    QAction *actToggleCamPreview_;
    QAction *actTogglePlaybackInterface_;

    QAction *actExecCurrentLine_;
    QAction *actExecPreviousLine_;
    QAction *actExecNextLine_;
    QAction *actToggleMultiLine_;

    QAction *actCommScan_;
    QAction *actToggleAutoCommScan_;

    QAction *actStartServer_;
    QAction *actStopServer_;
    QAction *actConnectToServer_;
    QAction *actDisconnectFromServer_;

    QAction *actOpenSettings_;

private slots:
    /* on action */
    void onActNewFile();
    void onActOpenFile();
    void onActSaveFile();
    void onActSaveFileAs();

    void onActExecCurrentLine();
    void onActExecPreviousLine();
    void onActExecNextLine();
    void onActToggleMultiLine(bool on);

    void onActCommScan();
    void onActToggleAutoCommScan(bool on);

    void onActStartServer();
    void onActConnectToServer();
    void onActDisconnectFromServer();

    void onActOpenSettings();

    void onActToggleCamPreview(bool on);
    void onActTogglePlaybackInterface(bool on);

    /* on other signal */
    void onLineStarted(const int &lineNum);
    void onCommandError(CommandErr err);
    void onConnectionError(ConnectionErr err);

    void onCommandsProcessed();
    void onChangePreview(QByteArray data, const unsigned long &size);

    void onPBarChanged(int imgIndex);
    void onPBarReleased();
    void onImgIndicesUpdated();

    void onEditorContentChanged();
};
