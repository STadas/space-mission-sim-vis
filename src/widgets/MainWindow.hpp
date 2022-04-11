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

/**
 * The main window class. This is the main class of the whole application
 */
class MainWindow : public QMainWindow, public ISavableLoadable
{
    Q_OBJECT

public:
    /**
     * Constructor
     * @param parent Parent widget
     */
    MainWindow(QWidget *parent = nullptr);
    /**
     * Destructor
     */
    ~MainWindow();

    /**
     * Override the close event to do some stuff before the destructor, like
     * saving geometry and state of the window
     * @param event Event passed by the GUI framework
     */
    void closeEvent(QCloseEvent *event) override;

    /**
     * Save the window's geometry and state
     */
    void save() override;
    /**
     *  Load the windows's geometry and state
     */
    void load() override;

    /**
     *  Initialize dock widgets
     */
    void initDockWidgets();
    /**
     *  Initialize signal connecions
     */
    void initSignalConnections();
    /**
     *  Initialize actions
     */
    void initActions();
    /**
     *  Initialize menus that hold actions
     */
    void initMenus();
    /**
     *  Initialize toolbars that hold actions
     */
    void initToolBars();
    /**
     *  Initialize the playback interface that holds actions
     */
    void initPlayBackInterface();

    Settings *const settings_;
    Resources *const resources_;

    Editor *editor_;
    CoordsVis *coordsVis_;
    CamPreview *camPreview_;
    PlaybackInterface *playbackInterface_;
    LogsView *logsView_;

    QDockWidget *dockCoordsVis_;
    QDockWidget *dockCamPreview_;
    QDockWidget *dockPlaybackInterface_;
    QDockWidget *dockLogsView_;

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
    /**
     * Start a new file
     * @see actNewFile_
     */
    void onActNewFile();
    /**
     * Open a file
     * @see actOpenFile_
     */
    void onActOpenFile();
    /**
     * Save the current file
     * @see actSaveFile_
     */
    void onActSaveFile();
    /**
     * Open file browser to choose how to save the current file
     * @see actSaveFileAs_
     */
    void onActSaveFileAs();

    /**
     * Execute the command of the currently active line in the editor
     * @see actExecCurrentLine_
     */
    void onActExecCurrentLine();
    /**
     * Execute the command of the line before the currently active one in the
     * editor
     * @see actExecPreviousLine_
     */
    void onActExecPreviousLine();
    /**
     * Execute the command of the line after the currently active one in the
     * editor
     * @see actExecNextLine_
     */
    void onActExecNextLine();
    /**
     * Start or pause multi-line stepping
     * @param on Whether to start or pause multi-line stepping
     * @see actToggleMultiLine_
     * */
    void onActToggleMultiLine(bool on);

    /**
     * Scan editor for commands that fit the criteria for widgets like the
     * progress bar
     * @see actCommScan_
     */
    void onActCommScan();
    /**
     * Toggle automatic command scanning, do a scan if turned on
     * @param on Whether to enable or disable auto command scanning
     * @see actToggleAutoCommScan_
     * */
    void onActToggleAutoCommScan(bool on);

    /**
     * Start a server process
     * @see actStartServer_
     */
    void onActStartServer();
    /**
     * Connect to a server
     * @see actConnectToServer_
     */
    void onActConnectToServer();
    /**
     * Disconnect from the currently connected server
     * @see actDisconnectFromServer_
     */
    void onActDisconnectFromServer();

    /**
     * Open the settings dialog
     * @see actOpenSettings_
     */
    void onActOpenSettings();

    /**
     * Show or hide the 3D coordinates visualisation
     * @param on Whether to show or hide the widget
     * @see actToggleCoordsVis_
     */
    void onActToggleCoordsVis(bool on);
    /**
     * Show or hide the camera preview
     * @param on Whether to show or hide the widget
     * @see actToggleCamPreview_
     */
    void onActToggleCamPreview(bool on);
    /**
     * Show or hide the playback interface
     * @param on Whether to show or hide the widget
     * @see actTogglePlaybackInterface_
     */
    void onActTogglePlaybackInterface(bool on);
    /**
     * Show or hide the server logs
     * @param on Whether to show or hide the widget
     * @see actToggleLogsView_
     */
    void onActToggleLogsView(bool on);

    /**
     * Open the manual
     * @see actOpenManual_
     */
    void onActOpenManual();
    /**
     * Open the About dialog
     * @see actOpenAbout_
     */
    void onActOpenAbout();

    /**
     * Update editor's active line and other widgets
     * @param lineNum line number to update in the editor and to find the
     * appropriate cam point
     * @see MainWindow::initSignalConnections
     */
    void onLineStarted(const unsigned int &lineNum);

    /**
     * Restore write access in the editor and update appropriate actions
     * @see MainWindow::initSignalConnections
     */
    void onCommandsProcessed();

    /**
     * Update active line in the text editor, coords vis and execute the command
     * in the new active line
     * @param idx Index to update in the coords vis and to find the appropriate
     * line number
     * @see MainWindow::initSignalConnections
     */
    void onPBarChanged(int idx);
    /**
     * Update active line in the text editor, coords vis and execute the command
     * in the new active line
     * @see MainWindow::initSignalConnections
     */
    void onPBarReleased();
    /**
     * Update the maximum value in the progress bar and points in the coords vis
     * @see MainWindow::initSignalConnections
     */
    void onCamPointsUpdated();

    /**
     * Scan commands if auto command scan is on
     * @see MainWindow::initSignalConnections
     */
    void onEditorContentChanged();

    /**
     * Display error message
     * @param err Error message enum
     * @see MainWindow::initSignalConnections
     */
    void onCommandError(CommandErr err);
    /**
     * Display error message
     * @param err Error message enum
     * @see MainWindow::initSignalConnections
     */
    void onConnectionError(ConnectionErr err);
    /**
     * Display error message
     * @param err Error message enum
     * @see MainWindow::initSignalConnections
     */
    void onServerProcessError(QProcess::ProcessError err);
    /**
     * Display error message
     * @param err Error message enum
     * @see MainWindow::initSignalConnections
     */
    void onFileError(FileErr err);
};
