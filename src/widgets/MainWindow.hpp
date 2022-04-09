#pragma once

#include <QtConcurrent>
#include <QtCore>
#include <QtWidgets>

#include "common/MessageController.hpp"
#include "common/PanguServerProcess.hpp"
#include "common/PingWorker.hpp"
#include "common/PreviewWorker.hpp"
#include "common/Resources.hpp"
#include "common/VBoxLayout.hpp"
#include "common/settings/Settings.hpp"
#include "interfaces/ISavableLoadable.hpp"
#include "widgets/Action.hpp"
#include "widgets/CamPreview.hpp"
#include "widgets/ConnectionDisplay.hpp"
#include "widgets/CoordsVis.hpp"
#include "widgets/Editor.hpp"
#include "widgets/LogsView.hpp"
#include "widgets/MenuSeparator.hpp"
#include "widgets/PlaybackInterface.hpp"
#include "widgets/RecentsMenu.hpp"
#include "widgets/dialogs/AboutDialog.hpp"
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

    void initDockWidgets();
    void initSignalConnections();
    void initActions();
    void initMenus();
    void initToolBars();
    void initPlayBackInterface();

    Settings *const settings_;
    Resources *const resources_;

    Editor *editor_;
    CoordsVis *coordsVis_;
    CamPreview *camPreview_;
    PlaybackInterface *playbackInterface_;
    ProgressBar *progressBar_;
    LogsView *logsView_;

    QDockWidget *dockCoordsVis_;
    QDockWidget *dockCamPreview_;
    QDockWidget *dockPlaybackInterface_;
    QDockWidget *dockLogsView_;

    bool autoCommScan_;

    MessageController *messageController_;

    PanguServerProcess *serverProcess_;

    PingWorker *pingWorker_;
    QThread *pingWorkerThread_;
    PreviewWorker *previewWorker_;
    QThread *previewWorkerThread_;

    ConnectionDisplay *connectionDisplay_;

    RecentsMenu *recentsMenu_;
    QMenu *fileMenu_;
    QMenu *editMenu_;
    QMenu *viewMenu_;
    QMenu *toolsMenu_;
    QMenu *commandsMenu_;
    QMenu *serverMenu_;
    QMenu *helpMenu_;

    Action *actNewFile_;
    Action *actOpenFile_;
    Action *actSaveFile_;
    Action *actSaveFileAs_;

    Action *actUndo_;
    Action *actRedo_;
    Action *actCut_;
    Action *actCopy_;
    Action *actPaste_;

    Action *actToggleCoordsVis_;
    Action *actToggleCamPreview_;
    Action *actTogglePlaybackInterface_;
    Action *actToggleLogsView_;

    Action *actExecCurrentLine_;
    Action *actExecPreviousLine_;
    Action *actExecNextLine_;
    Action *actToggleMultiLine_;

    Action *actCommScan_;
    Action *actToggleAutoCommScan_;

    Action *actStartServer_;
    Action *actStopServer_;
    Action *actConnectToServer_;
    Action *actDisconnectFromServer_;

    Action *actOpenSettings_;

    Action *actOpenManual_;
    Action *actOpenAbout_;

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

    void onActToggleCoordsVis(bool on);
    void onActToggleCamPreview(bool on);
    void onActTogglePlaybackInterface(bool on);
    void onActToggleLogsView(bool on);

    void onActOpenManual();
    void onActOpenAbout();

    /* on other signal */
    void onLineStarted(const unsigned int &lineNum);

    void onCommandsProcessed();
    void onChangePreview(QByteArray data);

    void onPBarChanged(int idx);
    void onPBarReleased();
    void onCamPointsUpdated();

    void onEditorContentChanged();

    void onCommandError(CommandErr err);
    void onConnectionError(ConnectionErr err);
    void onServerProcessError(QProcess::ProcessError err);
    void onFileError(FileErr err);
};
