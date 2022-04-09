#include "MainWindow.hpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , settings_(new Settings(this))
    , resources_(new Resources(this))
    , camPreview_(new CamPreview(this))
    , logsView_(new LogsView(this))
    , autoCommScan_(false)
    , messageController_(new MessageController(this))
    , pingWorker_(new PingWorker)  // no parent so it can moveToThread
    , pingWorkerThread_(new QThread(this))
    , connectionDisplay_(new ConnectionDisplay(this, this->resources_))
{
    this->settings_->load();

    this->previewWorker_ =
        new PreviewWorker(this->settings_);  // no parent so it can moveToThread
    this->previewWorkerThread_ = new QThread(this);

    this->editor_ = new Editor(this, this->settings_);
    this->coordsVis_ = new CoordsVis(this, this->settings_, this->resources_);
    this->serverProcess_ = new PanguServerProcess(this, this->settings_);
    this->playbackInterface_ = new PlaybackInterface(this, this->settings_);
    this->progressBar_ = this->playbackInterface_->progressBar_;

    /* As weird as this is, it needs to be done for us to be able to use the
     * enums with signals and slots. */
    qRegisterMetaType<CommandErr>("CommandErr");
    qRegisterMetaType<ConnectionErr>("ConnectionErr");
    qRegisterMetaType<FileErr>("FileErr");
    qRegisterMetaType<FileQuestion>("FileQuestion");

    this->pingWorker_->moveToThread(this->pingWorkerThread_);
    this->pingWorkerThread_->start();
    emit this->pingWorker_->start();

    this->previewWorker_->moveToThread(this->previewWorkerThread_);
    this->previewWorkerThread_->start();

    this->setCentralWidget(new QWidget(this));
    this->setStatusBar(new QStatusBar(this));
    this->setMenuBar(new QMenuBar(this));

    this->statusBar()->addPermanentWidget(this->connectionDisplay_);

    this->centralWidget()->setLayout(new VBoxLayout);
    this->centralWidget()->layout()->addWidget(this->editor_);

    this->initDockWidgets();
    this->initActions();
    this->initMenus();
    this->initToolBars();
    this->initPlayBackInterface();
    this->initSignalConnections();

    this->load();
}

MainWindow::~MainWindow()
{
    qDebug() << "Waiting for threads...";

    this->pingWorker_->stop();
    this->pingWorkerThread_->quit();
    this->pingWorkerThread_->wait();

    this->previewWorkerThread_->quit();
    this->previewWorkerThread_->wait();

    qDebug() << "Bye";
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    this->save();
    this->settings_->save();
    QMainWindow::closeEvent(event);
}

void MainWindow::save()
{
    this->settings_->geometryMainWindow.setValue(this->saveGeometry());
    this->settings_->stateMainWindow.setValue(this->saveState());
}

void MainWindow::load()
{
    if (this->settings_->geometryMainWindow.value() != QByteArray())
        this->restoreGeometry(this->settings_->geometryMainWindow.value());

    if (this->settings_->stateMainWindow.value() != QByteArray())
        this->restoreState(this->settings_->stateMainWindow.value());
}

void MainWindow::initDockWidgets()
{
    this->dockCoordsVis_ =
        new QDockWidget("3D coordinates visualisation", this);
    this->dockCoordsVis_->setObjectName("dockCoordsVis");
    this->dockCoordsVis_->setWidget(this->coordsVis_);
    this->addDockWidget(Qt::RightDockWidgetArea, this->dockCoordsVis_);

    this->dockCamPreview_ = new QDockWidget("Camera preview", this);
    this->dockCamPreview_->setObjectName("dockCamPreview");
    this->dockCamPreview_->setWidget(this->camPreview_);
    this->addDockWidget(Qt::RightDockWidgetArea, this->dockCamPreview_);

    this->dockPlaybackInterface_ = new QDockWidget("Playback interface", this);
    this->dockPlaybackInterface_->setObjectName("dockPlaybackInterface");
    this->dockPlaybackInterface_->setWidget(this->playbackInterface_);
    this->addDockWidget(Qt::RightDockWidgetArea, this->dockPlaybackInterface_);

    this->dockLogsView_ = new QDockWidget("Server log", this);
    this->dockLogsView_->setObjectName("dockLogsView");
    this->dockLogsView_->setWidget(this->logsView_);
    this->addDockWidget(Qt::BottomDockWidgetArea, this->dockLogsView_);
}

void MainWindow::initSignalConnections()
{
    QObject::connect(this->pingWorker_, &PingWorker::pinged,
                     this->previewWorker_, &PreviewWorker::ping);

    QObject::connect(this->previewWorker_, &PreviewWorker::lineStarted, this,
                     &MainWindow::onLineStarted);

    QObject::connect(this->previewWorker_,
                     qOverload<CommandErr>(&PreviewWorker::error), this,
                     &MainWindow::onCommandError);

    QObject::connect(this->previewWorker_,
                     qOverload<ConnectionErr>(&PreviewWorker::error), this,
                     &MainWindow::onConnectionError);

    QObject::connect(this->previewWorker_, &PreviewWorker::commandsProcessed,
                     this, &MainWindow::onCommandsProcessed);

    QObject::connect(this->previewWorker_, &PreviewWorker::changePreview, this,
                     &MainWindow::onChangePreview);

    QObject::connect(this->previewWorker_, &PreviewWorker::camPointsUpdated,
                     this, &MainWindow::onCamPointsUpdated);

    QObject::connect(this->previewWorker_, &PreviewWorker::connected, this,
                     [this] {
                         this->connectionDisplay_->updateStatus(
                             ConnectionDisplay::Status::Connected);
                     });

    QObject::connect(this->previewWorker_, &PreviewWorker::disconnected, this,
                     [this] {
                         this->connectionDisplay_->updateStatus(
                             ConnectionDisplay::Status::Disconnected);
                     });

    QObject::connect(this->serverProcess_, &PanguServerProcess::output,
                     this->logsView_, &LogsView::appendPlainText);

    QObject::connect(this->serverProcess_, &PanguServerProcess::error, this,
                     &MainWindow::onServerProcessError);

    QObject::connect(this->editor_->document(), &QTextDocument::contentsChanged,
                     this, &MainWindow::onEditorContentChanged);

    QObject::connect(this->editor_, &Editor::recentsUpdated, this->recentsMenu_,
                     &RecentsMenu::updateActions);

    QObject::connect(this->progressBar_, &ProgressBar::valueChanged, this,
                     &MainWindow::onPBarChanged);

    QObject::connect(this->progressBar_, &ProgressBar::sliderReleased, this,
                     &MainWindow::onPBarReleased);

    QObject::connect(this->recentsMenu_, &RecentsMenu::error, this,
                     &MainWindow::onFileError);
}

void MainWindow::initActions()
{
    this->actNewFile_ = new Action("New flight file", this);
    this->actNewFile_->setShortcuts(QKeySequence::New);
    this->actNewFile_->setStatusTip("Start editing a new flight file");
    this->actNewFile_->setIcon(this->resources_->iconDocumentNew);
    QObject::connect(this->actNewFile_, &QAction::triggered, this,
                     &MainWindow::onActNewFile);

    this->actOpenFile_ = new Action("Open flight file", this);
    this->actOpenFile_->setShortcuts(QKeySequence::Open);
    this->actOpenFile_->setStatusTip("Open a flight file to edit");
    this->actOpenFile_->setIcon(this->resources_->iconDocumentOpen);
    QObject::connect(this->actOpenFile_, &QAction::triggered, this,
                     &MainWindow::onActOpenFile);

    this->actSaveFile_ = new Action("Save flight file", this);
    this->actSaveFile_->setShortcuts(QKeySequence::Save);
    this->actSaveFile_->setStatusTip("Save the currently opened flight file");
    this->actSaveFile_->setIcon(this->resources_->iconDocumentSave);
    QObject::connect(this->actSaveFile_, &QAction::triggered, this,
                     &MainWindow::onActSaveFile);

    this->actSaveFileAs_ = new Action("Save as flight file", this);
    this->actSaveFileAs_->setShortcuts(QKeySequence::SaveAs);
    this->actSaveFileAs_->setStatusTip("Save as a new flight file");
    this->actSaveFileAs_->setIcon(this->resources_->iconDocumentSaveAs);
    QObject::connect(this->actSaveFileAs_, &QAction::triggered, this,
                     &MainWindow::onActSaveFileAs);

    this->actExecCurrentLine_ = new Action("Execute line", this);
    this->actExecCurrentLine_->setStatusTip(
        "Execute the command in the currently active line");
    this->actExecCurrentLine_->setIcon(this->resources_->iconCursorExecute);
    QObject::connect(this->actExecCurrentLine_, &QAction::triggered, this,
                     &MainWindow::onActExecCurrentLine);

    this->actExecPreviousLine_ = new Action("Execute previous line", this);
    this->actExecPreviousLine_->setStatusTip(
        "Execute the command in the previous line, skipping empty ones");
    this->actExecPreviousLine_->setIcon(this->resources_->iconSkipBackward);
    QObject::connect(this->actExecPreviousLine_, &QAction::triggered, this,
                     &MainWindow::onActExecPreviousLine);

    this->actExecNextLine_ = new Action("Execute next line", this);
    this->actExecNextLine_->setStatusTip(
        "Execute the command in the next line, skipping empty ones");
    this->actExecNextLine_->setIcon(this->resources_->iconSkipForward);
    QObject::connect(this->actExecNextLine_, &QAction::triggered, this,
                     &MainWindow::onActExecNextLine);

    this->actToggleMultiLine_ = new Action("Step through lines", this);
    this->actToggleMultiLine_->setStatusTip(
        "Step through and execute all commands (with delay) starting with the "
        "currently active line");
    this->actToggleMultiLine_->setIcon(this->resources_->iconPlaybackStart);
    this->actToggleMultiLine_->setCheckable(true);
    this->actToggleMultiLine_->setChecked(false);
    QObject::connect(this->actToggleMultiLine_, &QAction::toggled, this,
                     &MainWindow::onActToggleMultiLine);

    this->actCommScan_ = new Action("Scan all commands", this);
    this->actCommScan_->setStatusTip("Scan all commands to update components "
                                     "like the playback progress bar");
    this->actCommScan_->setIcon(this->resources_->iconScan);
    QObject::connect(this->actCommScan_, &QAction::triggered, this,
                     &MainWindow::onActCommScan);

    this->actToggleAutoCommScan_ =
        new Action("Toggle auto command scanning", this);
    this->actToggleAutoCommScan_->setStatusTip(
        "Toggle automatic scanning of all commands when editing to update "
        "components like the playback progress bar (slow with large files!)");
    this->actToggleAutoCommScan_->setIcon(this->resources_->iconRecurring);
    this->actToggleAutoCommScan_->setCheckable(true);
    this->actToggleAutoCommScan_->setChecked(false);
    QObject::connect(this->actToggleAutoCommScan_, &QAction::toggled, this,
                     &MainWindow::onActToggleAutoCommScan);

    this->actStartServer_ = new Action("Start server", this);
    this->actStartServer_->setStatusTip("Start a server to send commands to");
    this->actStartServer_->setIcon(this->resources_->iconDisplay);
    QObject::connect(this->actStartServer_, &QAction::triggered, this,
                     &MainWindow::onActStartServer);

    this->actConnectToServer_ = new Action("Connect to server", this);
    this->actConnectToServer_->setStatusTip(
        "Connect to the currently used server");
    this->actConnectToServer_->setIcon(this->resources_->iconNetworkConnect);
    QObject::connect(this->actConnectToServer_, &QAction::triggered, this,
                     &MainWindow::onActConnectToServer);

    this->actDisconnectFromServer_ = new Action("Disconnect from server", this);
    this->actDisconnectFromServer_->setStatusTip(
        "Disconnect from the currently used server");
    this->actDisconnectFromServer_->setIcon(
        this->resources_->iconNetworkDisconnect);
    QObject::connect(this->actDisconnectFromServer_, &QAction::triggered, this,
                     &MainWindow::onActDisconnectFromServer);

    this->actOpenSettings_ = new Action("Settings", this);
    this->actOpenSettings_->setStatusTip("Open the settings window");
    this->actOpenSettings_->setIcon(this->resources_->iconConfigure);
    QObject::connect(this->actOpenSettings_, &QAction::triggered, this,
                     &MainWindow::onActOpenSettings);

    this->actToggleCoordsVis_ = new Action("Coordinates visualisation", this);
    this->actToggleCoordsVis_->setStatusTip(
        "Toggle visibility of the coordinates visualisation");
    this->actToggleCoordsVis_->setCheckable(true);
    this->actToggleCoordsVis_->setChecked(this->dockCoordsVis_->isVisible());
    QObject::connect(this->actToggleCoordsVis_, &QAction::toggled, this,
                     &MainWindow::onActToggleCoordsVis);
    QObject::connect(this->dockCoordsVis_, &QDockWidget::visibilityChanged,
                     this, [this]() {
                         this->actToggleCoordsVis_->setChecked(
                             !this->dockCoordsVis_->isHidden());
                     });

    this->actToggleCamPreview_ = new Action("Camera preview", this);
    this->actToggleCamPreview_->setStatusTip(
        "Toggle visibility of the camera preview");
    this->actToggleCamPreview_->setCheckable(true);
    this->actToggleCamPreview_->setChecked(this->dockCamPreview_->isVisible());
    QObject::connect(this->actToggleCamPreview_, &QAction::toggled, this,
                     &MainWindow::onActToggleCamPreview);
    QObject::connect(this->dockCamPreview_, &QDockWidget::visibilityChanged,
                     this, [this]() {
                         this->actToggleCamPreview_->setChecked(
                             !this->dockCamPreview_->isHidden());
                     });

    this->actTogglePlaybackInterface_ = new Action("Playback interface", this);
    this->actTogglePlaybackInterface_->setStatusTip(
        "Toggle visibility of the playback interface");
    this->actTogglePlaybackInterface_->setCheckable(true);
    this->actTogglePlaybackInterface_->setChecked(
        this->dockPlaybackInterface_->isVisible());
    QObject::connect(this->actTogglePlaybackInterface_, &QAction::toggled, this,
                     &MainWindow::onActTogglePlaybackInterface);
    QObject::connect(this->dockPlaybackInterface_,
                     &QDockWidget::visibilityChanged, this, [this]() {
                         this->actTogglePlaybackInterface_->setChecked(
                             !this->dockPlaybackInterface_->isHidden());
                     });

    this->actToggleLogsView_ = new Action("Server log", this);
    this->actToggleLogsView_->setStatusTip(
        "Toggle visibility of the server log");
    this->actToggleLogsView_->setCheckable(true);
    this->actToggleLogsView_->setChecked(this->dockLogsView_->isVisible());
    QObject::connect(this->actToggleLogsView_, &QAction::toggled, this,
                     &MainWindow::onActToggleLogsView);
    QObject::connect(this->dockLogsView_, &QDockWidget::visibilityChanged, this,
                     [this]() {
                         this->actToggleLogsView_->setChecked(
                             !this->dockLogsView_->isHidden());
                     });

    this->actUndo_ = new Action("Undo", this);
    this->actUndo_->setShortcuts(QKeySequence::Undo);
    this->actUndo_->setStatusTip("Undo the latest edit in the editor");
    this->actUndo_->setIcon(this->resources_->iconEditUndo);
    QObject::connect(this->actUndo_, &QAction::triggered, this->editor_,
                     &QPlainTextEdit::undo);

    this->actRedo_ = new Action("Redo", this);
    this->actRedo_->setShortcuts(QKeySequence::Redo);
    this->actRedo_->setStatusTip("Redo the latest edit in the editor");
    this->actRedo_->setIcon(this->resources_->iconEditRedo);
    QObject::connect(this->actRedo_, &QAction::triggered, this->editor_,
                     &QPlainTextEdit::redo);

    this->actCut_ = new Action("Cut", this);
    this->actCut_->setShortcuts(QKeySequence::Cut);
    this->actCut_->setStatusTip(
        "Cut the currently selected text in the editor");
    this->actCut_->setIcon(this->resources_->iconEditCut);
    QObject::connect(this->actCut_, &QAction::triggered, this->editor_,
                     &QPlainTextEdit::cut);

    this->actCopy_ = new Action("Copy", this);
    this->actCopy_->setShortcuts(QKeySequence::Copy);
    this->actCopy_->setStatusTip(
        "Copy the currently selected text in the editor");
    this->actCopy_->setIcon(this->resources_->iconEditCopy);
    QObject::connect(this->actCopy_, &QAction::triggered, this->editor_,
                     &QPlainTextEdit::copy);

    this->actPaste_ = new Action("Paste", this);
    this->actPaste_->setShortcuts(QKeySequence::Paste);
    this->actPaste_->setStatusTip(
        "Paste text from the clipboard under the editor cursor");
    this->actPaste_->setIcon(this->resources_->iconEditPaste);
    QObject::connect(this->actPaste_, &QAction::triggered, this->editor_,
                     &QPlainTextEdit::paste);

    this->actOpenManual_ = new Action("Manual", this);
    this->actOpenManual_->setStatusTip("Read the manual");
    this->actOpenManual_->setIcon(this->resources_->iconQuestion);
    QObject::connect(this->actOpenManual_, &QAction::triggered, this,
                     &MainWindow::onActOpenManual);

    this->actOpenAbout_ = new Action("About", this);
    this->actOpenAbout_->setStatusTip("About the application and licenses");
    this->actOpenAbout_->setIcon(this->resources_->iconInfo);
    QObject::connect(this->actOpenAbout_, &QAction::triggered, this,
                     &MainWindow::onActOpenAbout);
}

void MainWindow::initMenus()
{
    this->fileMenu_ = new QMenu("File", this);
    this->menuBar()->addMenu(this->fileMenu_);
    this->fileMenu_->addActions({
        this->actNewFile_,
        new MenuSeparator(this),
        this->actOpenFile_,
    });

    this->recentsMenu_ =
        new RecentsMenu(this, this->settings_, this->messageController_);
    this->recentsMenu_->updateActions(this->editor_);
    this->fileMenu_->addMenu(this->recentsMenu_);

    this->fileMenu_->addActions({
        new MenuSeparator(this),
        this->actSaveFile_,
        this->actSaveFileAs_,
    });

    this->editMenu_ = new QMenu("Edit", this);
    this->menuBar()->addMenu(this->editMenu_);
    this->editMenu_->addActions({
        this->actUndo_,
        this->actRedo_,
        new MenuSeparator(this),
        this->actCut_,
        this->actCopy_,
        this->actPaste_,
    });

    this->viewMenu_ = new QMenu("View", this);
    this->menuBar()->addMenu(this->viewMenu_);
    this->viewMenu_->addActions({
        this->actToggleCoordsVis_,
        this->actToggleCamPreview_,
        this->actTogglePlaybackInterface_,
        this->actToggleLogsView_,
    });

    this->toolsMenu_ = new QMenu("Tools", this);
    this->menuBar()->addMenu(this->toolsMenu_);
    this->toolsMenu_->addActions({
        this->actOpenSettings_,
        new MenuSeparator(this),
    });

    this->commandsMenu_ = new QMenu("Commands", this);
    this->toolsMenu_->addMenu(this->commandsMenu_);
    this->commandsMenu_->addActions({
        this->actExecCurrentLine_,
        this->actExecPreviousLine_,
        this->actExecNextLine_,
        this->actToggleMultiLine_,
        new MenuSeparator(this),
        this->actCommScan_,
        this->actToggleAutoCommScan_,
    });

    this->editor_->setExtraContextMenuActions({
        this->actExecCurrentLine_,
        this->actToggleMultiLine_,
        this->actCommScan_,
        new MenuSeparator(this),
    });

    this->serverMenu_ = new QMenu("Server", this);
    this->toolsMenu_->addMenu(this->serverMenu_);
    this->serverMenu_->addActions({
        this->actStartServer_,
        this->actConnectToServer_,
        this->actDisconnectFromServer_,
    });

    this->helpMenu_ = new QMenu("Help", this);
    this->menuBar()->addMenu(this->helpMenu_);
    this->helpMenu_->addActions({
        this->actOpenManual_,
        this->actOpenAbout_,
    });
}

void MainWindow::initToolBars()
{
    QToolBar *fileToolBar = new QToolBar(this);
    this->addToolBar(fileToolBar);
    fileToolBar->setObjectName("fileToolBar");
    fileToolBar->addAction(this->actNewFile_);
    fileToolBar->addAction(this->actOpenFile_);
    fileToolBar->addAction(this->actSaveFile_);

    QToolBar *commandToolBar = new QToolBar(this);
    this->addToolBar(commandToolBar);
    commandToolBar->setObjectName("commandToolBar");
    commandToolBar->addAction(this->actToggleAutoCommScan_);
    commandToolBar->addAction(this->actCommScan_);
    commandToolBar->addAction(this->actExecCurrentLine_);
    commandToolBar->addAction(this->actExecPreviousLine_);
    commandToolBar->addAction(this->actToggleMultiLine_);
    commandToolBar->addAction(this->actExecNextLine_);

    QToolBar *serverToolBar = new QToolBar(this);
    this->addToolBar(serverToolBar);
    serverToolBar->setObjectName("serverToolBar");
    serverToolBar->addAction(this->actStartServer_);
    serverToolBar->addAction(this->actConnectToServer_);
    serverToolBar->addAction(this->actDisconnectFromServer_);
}

void MainWindow::initPlayBackInterface()
{
    this->playbackInterface_->addButton(this->actCommScan_);
    this->playbackInterface_->addButton(this->actExecCurrentLine_);
    this->playbackInterface_->addButton(this->actExecPreviousLine_);
    this->playbackInterface_->addButton(this->actToggleMultiLine_);
    this->playbackInterface_->addButton(this->actExecNextLine_);
}

void MainWindow::onActNewFile()
{
    if (this->editor_->isModified())
    {
        QMessageBox::StandardButton answer =
            this->messageController_->question(FileQuestion::FileNew, this);
        if (answer == QMessageBox::No)
        {
            return;
        }
    }

    this->editor_->clear();
}

void MainWindow::onActOpenFile()
{
    if (this->editor_->isModified())
    {
        QMessageBox::StandardButton answer =
            this->messageController_->question(FileQuestion::FileOpen, this);
        if (answer == QMessageBox::No)
        {
            return;
        }
    }

    if (this->editor_->load())
    {
        emit this->messageController_->error(FileErr::OpenFail, this);
    };
}

void MainWindow::onActSaveFileAs()
{
    if (this->editor_->saveAs())
    {
        emit this->messageController_->error(FileErr::SaveFail, this);
    }
}

void MainWindow::onActSaveFile()
{
    if (this->editor_->save())
    {
        emit this->messageController_->error(FileErr::SaveFail, this);
    }
}

void MainWindow::onActExecCurrentLine()
{
    if (!this->previewWorker_->previewLock()->available())
        return;

    this->onLineStarted(this->editor_->textCursor().blockNumber());
    emit this->previewWorker_->processCommands(this->editor_->activeLineText());
}

void MainWindow::onActExecPreviousLine()
{
    if (!this->previewWorker_->previewLock()->available())
        return;

    if (this->editor_->textCursor().blockNumber() == 0)
        return;

    this->onLineStarted(this->editor_->textCursor().blockNumber() - 1);
    emit this->previewWorker_->processCommands(this->editor_->activeLineText());
}

void MainWindow::onActExecNextLine()
{
    if (!this->previewWorker_->previewLock()->available())
        return;

    if (this->editor_->textCursor().blockNumber() ==
        this->editor_->blockCount() - 1)
        return;

    this->onLineStarted(this->editor_->textCursor().blockNumber() + 1);
    emit this->previewWorker_->processCommands(this->editor_->activeLineText());
}

void MainWindow::onActToggleMultiLine(bool on)
{
    if (on)
    {
        this->actToggleMultiLine_->setText("Stop stepping");
        this->actToggleMultiLine_->setStatusTip(
            "Stop the currently active command stepping");
        this->actToggleMultiLine_->setIcon(
            this->style()->standardIcon(QStyle::SP_MediaPause));

        this->editor_->setReadOnly(true);

        int startLine = this->editor_->textCursor().blockNumber();

        if (startLine == this->editor_->blockCount() - 1)
        {
            startLine = 0;
        }

        emit this->previewWorker_->processCommands(
            this->editor_->toPlainText(), startLine,
            this->settings_->commandsStepMsDelay.value());
    }
    else
    {
        if (!this->previewWorker_->previewLock()->available())
        {
            this->previewWorker_->cancelStepping();
            return;
        }
    }
}

void MainWindow::onActCommScan()
{
    emit this->previewWorker_->updateCamPoints(this->editor_->toPlainText());
}

void MainWindow::onActToggleAutoCommScan(bool on)
{
    this->autoCommScan_ = on;
    if (on)
    {
        emit this->previewWorker_->updateCamPoints(
            this->editor_->toPlainText());
    }
}

void MainWindow::onActStartServer()
{
    this->serverProcess_->start(
        this->settings_->serverPath.value(),
        this->settings_->serverFlags.value().split(" "));
}

void MainWindow::onActConnectToServer()
{
    emit this->previewWorker_->connect(this->settings_->serverAddress.value(),
                                       this->settings_->serverPort.value());
}

void MainWindow::onActDisconnectFromServer()
{
    emit this->previewWorker_->disconnect();
}

void MainWindow::onActOpenSettings()
{
    SettingsDialog *settingsDialog =
        new SettingsDialog(this, this->settings_, this->resources_);
    settingsDialog->show();
}

void MainWindow::onActToggleCoordsVis(bool on)
{
    if (on)
        this->dockCoordsVis_->show();
    else
        this->dockCoordsVis_->hide();
}

void MainWindow::onActToggleCamPreview(bool on)
{
    if (on)
        this->dockCamPreview_->show();
    else
        this->dockCamPreview_->hide();
}

void MainWindow::onActTogglePlaybackInterface(bool on)
{
    if (on)
        this->dockPlaybackInterface_->show();
    else
        this->dockPlaybackInterface_->hide();
}

void MainWindow::onActToggleLogsView(bool on)
{
    if (on)
        this->dockLogsView_->show();
    else
        this->dockLogsView_->hide();
}

void MainWindow::onActOpenManual()
{
    QDesktopServices::openUrl(this->resources_->manualUrl);
}

void MainWindow::onActOpenAbout()
{
    AboutDialog *aboutDialog = new AboutDialog(this, this->resources_);
    aboutDialog->show();
}

void MainWindow::onLineStarted(const unsigned int &lineNum)
{
    this->editor_->goToLine(lineNum);

    if (this->previewWorker_->camPoints().size() == 0)
        return;

    CamPoint camPoint =
        this->previewWorker_->camPoints()[this->progressBar_->value()];
    if (camPoint.lineNum() != lineNum)
    {
        /* try to find cam point by line number */
        auto camPoints = this->previewWorker_->camPoints();
        auto it = std::find_if(camPoints.begin(), camPoints.end(),
                               [lineNum](const CamPoint &point) {
                                   return point.lineNum() == lineNum;
                               });

        /* update progress bar and coords vis if found */
        if (it != camPoints.end())
        {
            unsigned int activeIdx = it - camPoints.begin();
            this->progressBar_->blockSignals(true);
            this->progressBar_->setValue(activeIdx);
            this->progressBar_->blockSignals(false);

            this->coordsVis_->updateActive(activeIdx);
        }
    }
}

void MainWindow::onCommandsProcessed()
{
    this->editor_->setReadOnly(false);
    this->actToggleMultiLine_->setText("Step through lines");
    this->actToggleMultiLine_->setStatusTip(
        "Step through and execute all commands (with delay) starting with the "
        "currently active line");
    this->actToggleMultiLine_->setIcon(
        this->style()->standardIcon(QStyle::SP_MediaPlay));
    this->actToggleMultiLine_->setChecked(false);
}

void MainWindow::onChangePreview(QByteArray data)
{
    this->camPreview_->showPreview(data);
}

void MainWindow::onPBarChanged(int idx)
{
    this->previewWorker_->cancelStepping();

    CamPoint camPoint = this->previewWorker_->camPoints()[idx];

    if (this->previewWorker_->previewLock()->available())
    {
        this->editor_->goToLine(camPoint.lineNum());
        this->coordsVis_->updateActive(idx);

        emit this->previewWorker_->processCommands(
            this->editor_->activeLineText());
    }
    else
    {
        this->editor_->goToLine(camPoint.lineNum());
        this->coordsVis_->updateActive(idx);
    }
}

void MainWindow::onPBarReleased()
{
    this->previewWorker_->cancelStepping();

    unsigned int idx = this->progressBar_->value();
    CamPoint camPoint = this->previewWorker_->camPoints()[idx];

    this->coordsVis_->updateActive(idx);
    this->editor_->goToLine(camPoint.lineNum());
    emit this->previewWorker_->processCommands(this->editor_->activeLineText());
}

void MainWindow::onCamPointsUpdated()
{
    if (this->previewWorker_->camPoints().size() > 0)
    {
        this->progressBar_->setEnabled(true);
        this->progressBar_->setMaximum(
            this->previewWorker_->camPoints().size() - 1);
    }
    else
    {
        this->progressBar_->blockSignals(true);
        this->progressBar_->setMaximum(0);
        this->progressBar_->blockSignals(false);
        this->progressBar_->setEnabled(false);
    }
    this->coordsVis_->updatePoints(this->previewWorker_->camPoints());
    this->coordsVis_->updateActive(this->progressBar_->value());
}

void MainWindow::onEditorContentChanged()
{
    if (this->autoCommScan_)
        emit this->previewWorker_->updateCamPoints(
            this->editor_->toPlainText());
}

void MainWindow::onCommandError(CommandErr err)
{
    emit this->messageController_->error(err, this);
}

void MainWindow::onConnectionError(ConnectionErr err)
{
    emit this->messageController_->error(err, this);
}

void MainWindow::onServerProcessError(QProcess::ProcessError err)
{
    emit this->messageController_->error(err, this);
}

void MainWindow::onFileError(FileErr err)
{
    emit this->messageController_->error(err, this);
}
