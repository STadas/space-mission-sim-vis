#include "MainWindow.hpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , settings_(new Settings(this))
    , resources_(new Resources(this))
    , camPreview_(new CamPreview(this))
    , playbackInterface_(new PlaybackInterface(this))
    , progressBar_(this->playbackInterface_->progressBar_)
    , autoCommScan_(false)
    , messageController_(new MessageController(this))
    , previewWorker_(new PreviewWorker)  // no parent so it can moveToThread
    , previewWorkerThread_(new QThread(this))
    , serverProcess_(new PanguServerProcess(this))
{
    this->settings_->load();
    this->editor_ = new Editor(this, this->settings_);
    this->coordsVis_ = new CoordsVis(this, this->settings_, this->resources_);

    /* As weird as this is, it needs to be done for us to be able to use the
     * enums with signals and slots. Potential for a generated source code
     * file? */
    qRegisterMetaType<CommandErr>("CommandErr");
    qRegisterMetaType<ConnectionErr>("ConnectionErr");
    qRegisterMetaType<FileErr>("FileErr");
    qRegisterMetaType<FileQuestion>("FileQuestion");

    this->previewWorker_->moveToThread(this->previewWorkerThread_);
    this->previewWorkerThread_->start();

    this->setCentralWidget(new QWidget(this));
    this->setStatusBar(new QStatusBar(this));
    this->setMenuBar(new QMenuBar(this));

    this->centralWidget()->setLayout(new VBoxLayout);
    this->centralWidget()->layout()->addWidget(this->editor_);

    this->dockCoordsVis_ =
        new QDockWidget("3D Coordinates Visualisation", this);
    this->dockCoordsVis_->setObjectName("dockCoordsVis");
    this->dockCoordsVis_->setWidget(this->coordsVis_);
    this->addDockWidget(Qt::RightDockWidgetArea, this->dockCoordsVis_);

    this->dockCamPreview_ = new QDockWidget("Camera Preview", this);
    this->dockCamPreview_->setObjectName("dockCamPreview");
    this->dockCamPreview_->setWidget(this->camPreview_);
    this->addDockWidget(Qt::RightDockWidgetArea, this->dockCamPreview_);

    this->dockPlaybackInterface_ = new QDockWidget("Playback Interface", this);
    this->dockPlaybackInterface_->setObjectName("dockPlaybackInterface");
    this->dockPlaybackInterface_->setWidget(this->playbackInterface_);
    this->addDockWidget(Qt::RightDockWidgetArea, this->dockPlaybackInterface_);

    this->initSignalConnections();
    this->initActions();
    this->initMenus();
    this->initToolBars();
    this->initPlayBackInterface();

    this->load();
}

MainWindow::~MainWindow()
{
    this->previewWorkerThread_->quit();
    this->previewWorkerThread_->wait();
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

void MainWindow::initSignalConnections()
{
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

    QObject::connect(this->progressBar_, &ProgressBar::valueChanged, this,
                     &MainWindow::onPBarChanged);

    QObject::connect(this->progressBar_, &ProgressBar::sliderReleased, this,
                     &MainWindow::onPBarReleased);

    QObject::connect(this->previewWorker_, &PreviewWorker::camPointsUpdated,
                     this, &MainWindow::onCamPointsUpdated);

    QObject::connect(this->editor_->document(), &QTextDocument::contentsChanged,
                     this, &MainWindow::onEditorContentChanged);

    QObject::connect(this->serverProcess_, &PanguServerProcess::output, this,
                     [=](QString text) {});
}

void MainWindow::initActions()
{
    this->actNewFile_ = new QAction("New file", this);
    this->actNewFile_->setShortcuts(QKeySequence::New);
    this->actNewFile_->setStatusTip("Start editing a new file");
    this->actNewFile_->setIcon(this->resources_->iconDocumentNew);
    QObject::connect(this->actNewFile_, &QAction::triggered, this,
                     &MainWindow::onActNewFile);

    this->actOpenFile_ = new QAction("Open file", this);
    this->actOpenFile_->setShortcuts(QKeySequence::Open);
    this->actOpenFile_->setStatusTip("Open a file to edit");
    this->actOpenFile_->setIcon(this->resources_->iconDocumentOpen);
    QObject::connect(this->actOpenFile_, &QAction::triggered, this,
                     &MainWindow::onActOpenFile);

    this->actSaveFile_ = new QAction("Save", this);
    this->actSaveFile_->setShortcuts(QKeySequence::Save);
    this->actSaveFile_->setStatusTip("Save the currently opened file");
    this->actSaveFile_->setIcon(this->resources_->iconDocumentSave);
    QObject::connect(this->actSaveFile_, &QAction::triggered, this,
                     &MainWindow::onActSaveFile);

    this->actSaveFileAs_ = new QAction("Save as", this);
    this->actSaveFileAs_->setShortcuts(QKeySequence::SaveAs);
    this->actSaveFileAs_->setStatusTip("Save as a new file");
    this->actSaveFileAs_->setIcon(this->resources_->iconDocumentSaveAs);
    QObject::connect(this->actSaveFileAs_, &QAction::triggered, this,
                     &MainWindow::onActSaveFileAs);

    this->actExecCurrentLine_ = new QAction("Execute line", this);
    this->actExecCurrentLine_->setStatusTip(
        "Execute the command in the currently active line");
    this->actExecCurrentLine_->setIcon(this->resources_->iconCursorExecute);
    QObject::connect(this->actExecCurrentLine_, &QAction::triggered, this,
                     &MainWindow::onActExecCurrentLine);

    this->actExecPreviousLine_ = new QAction("Execute previous line", this);
    this->actExecPreviousLine_->setStatusTip(
        "Execute the command in the previous line, skipping empty ones");
    this->actExecPreviousLine_->setIcon(this->resources_->iconSkipBackward);
    QObject::connect(this->actExecPreviousLine_, &QAction::triggered, this,
                     &MainWindow::onActExecPreviousLine);

    this->actExecNextLine_ = new QAction("Execute next line", this);
    this->actExecNextLine_->setStatusTip(
        "Execute the command in the next line, skipping empty ones");
    this->actExecNextLine_->setIcon(this->resources_->iconSkipForward);
    QObject::connect(this->actExecNextLine_, &QAction::triggered, this,
                     &MainWindow::onActExecNextLine);

    this->actToggleMultiLine_ = new QAction("Step through lines", this);
    this->actToggleMultiLine_->setStatusTip(
        "Step through and execute all commands (with delay) starting with the "
        "currently active line");
    this->actToggleMultiLine_->setIcon(this->resources_->iconPlaybackStart);
    this->actToggleMultiLine_->setCheckable(true);
    this->actToggleMultiLine_->setChecked(false);
    QObject::connect(this->actToggleMultiLine_, &QAction::toggled, this,
                     &MainWindow::onActToggleMultiLine);

    this->actCommScan_ = new QAction("Scan all commands", this);
    this->actCommScan_->setStatusTip("Scan all commands to update components "
                                     "like the playback progress bar");
    this->actCommScan_->setIcon(this->resources_->iconScan);
    QObject::connect(this->actCommScan_, &QAction::triggered, this,
                     &MainWindow::onActCommScan);

    this->actToggleAutoCommScan_ =
        new QAction("Toggle auto command scanning", this);
    this->actToggleAutoCommScan_->setStatusTip(
        "Toggle automatic scanning of all commands when editing to update "
        "components like the playback progress bar (slow with large files!)");
    this->actToggleAutoCommScan_->setIcon(this->resources_->iconRecurring);
    this->actToggleAutoCommScan_->setCheckable(true);
    this->actToggleAutoCommScan_->setChecked(false);
    QObject::connect(this->actToggleAutoCommScan_, &QAction::toggled, this,
                     &MainWindow::onActToggleAutoCommScan);

    this->actStartServer_ = new QAction("Start server", this);
    this->actStartServer_->setStatusTip("Start a server to send commands to");
    this->actStartServer_->setIcon(this->resources_->iconDisplay);
    QObject::connect(this->actStartServer_, &QAction::triggered, this,
                     &MainWindow::onActStartServer);

    this->actConnectToServer_ = new QAction("Connect to server", this);
    this->actConnectToServer_->setStatusTip(
        "Connect to the currently used server");
    this->actConnectToServer_->setIcon(this->resources_->iconNetworkConnect);
    QObject::connect(this->actConnectToServer_, &QAction::triggered, this,
                     &MainWindow::onActConnectToServer);

    this->actDisconnectFromServer_ =
        new QAction("Disconnect from server", this);
    this->actDisconnectFromServer_->setStatusTip(
        "Disconnect from the currently used server");
    this->actDisconnectFromServer_->setIcon(
        this->resources_->iconNetworkDisconnect);
    QObject::connect(this->actDisconnectFromServer_, &QAction::triggered, this,
                     &MainWindow::onActDisconnectFromServer);

    this->actOpenSettings_ = new QAction("Settings", this);
    this->actOpenSettings_->setStatusTip("Open the settings window");
    this->actOpenSettings_->setIcon(this->resources_->iconConfigure);
    QObject::connect(this->actOpenSettings_, &QAction::triggered, this,
                     &MainWindow::onActOpenSettings);

    this->actToggleCoordsVis_ = new QAction("Coordinates visualisation", this);
    this->actToggleCoordsVis_->setStatusTip(
        "Toggle visibility of the coordinates visualisation");
    this->actToggleCoordsVis_->setCheckable(true);
    this->actToggleCoordsVis_->setChecked(this->dockCoordsVis_->isVisible());
    QObject::connect(this->actToggleCoordsVis_, &QAction::toggled, this,
                     &MainWindow::onActToggleCoordsVis);
    QObject::connect(this->dockCoordsVis_, &QDockWidget::visibilityChanged,
                     this->actToggleCoordsVis_, &QAction::setChecked);

    this->actToggleCamPreview_ = new QAction("Camera preview", this);
    this->actToggleCamPreview_->setStatusTip(
        "Toggle visibility of the camera preview");
    this->actToggleCamPreview_->setCheckable(true);
    this->actToggleCamPreview_->setChecked(this->dockCamPreview_->isVisible());
    QObject::connect(this->actToggleCamPreview_, &QAction::toggled, this,
                     &MainWindow::onActToggleCamPreview);
    QObject::connect(this->dockCamPreview_, &QDockWidget::visibilityChanged,
                     this->actToggleCamPreview_, &QAction::setChecked);

    this->actTogglePlaybackInterface_ = new QAction("Playback interface", this);
    this->actTogglePlaybackInterface_->setStatusTip(
        "Toggle visibility of the playback interface");
    this->actTogglePlaybackInterface_->setCheckable(true);
    this->actTogglePlaybackInterface_->setChecked(
        this->dockPlaybackInterface_->isVisible());
    QObject::connect(this->actTogglePlaybackInterface_, &QAction::toggled, this,
                     &MainWindow::onActTogglePlaybackInterface);
    QObject::connect(this->dockPlaybackInterface_,
                     &QDockWidget::visibilityChanged,
                     this->actTogglePlaybackInterface_, &QAction::setChecked);

    this->actUndo_ = new QAction("Undo", this);
    this->actUndo_->setStatusTip("Undo the latest edit in the editor");
    this->actUndo_->setIcon(this->resources_->iconEditUndo);
    QObject::connect(this->actUndo_, &QAction::triggered, this->editor_,
                     &QPlainTextEdit::undo);

    this->actRedo_ = new QAction("Redo", this);
    this->actRedo_->setStatusTip("Redo the latest edit in the editor");
    this->actRedo_->setIcon(this->resources_->iconEditRedo);
    QObject::connect(this->actRedo_, &QAction::triggered, this->editor_,
                     &QPlainTextEdit::redo);

    this->actCut_ = new QAction("Cut", this);
    this->actCut_->setStatusTip(
        "Cut the currently selected text in the editor");
    this->actCut_->setIcon(this->resources_->iconEditCut);
    QObject::connect(this->actCut_, &QAction::triggered, this->editor_,
                     &QPlainTextEdit::cut);

    this->actCopy_ = new QAction("Copy", this);
    this->actCopy_->setStatusTip(
        "Copy the currently selected text in the editor");
    this->actCopy_->setIcon(this->resources_->iconEditCopy);
    QObject::connect(this->actCopy_, &QAction::triggered, this->editor_,
                     &QPlainTextEdit::copy);

    this->actPaste_ = new QAction("Paste", this);
    this->actPaste_->setStatusTip(
        "Paste text from the clipboard under the editor cursor");
    this->actPaste_->setIcon(this->resources_->iconEditPaste);
    QObject::connect(this->actPaste_, &QAction::triggered, this->editor_,
                     &QPlainTextEdit::paste);
}

void MainWindow::initMenus()
{
    this->fileMenu_ = new QMenu("File", this);
    this->menuBar()->addMenu(this->fileMenu_);
    this->fileMenu_->addActions({
        this->actNewFile_,
    });
    this->fileMenu_->addSeparator();
    this->fileMenu_->addActions({
        this->actOpenFile_,
    });
    this->fileMenu_->addSeparator();
    this->fileMenu_->addActions({
        this->actSaveFile_,
        this->actSaveFileAs_,
    });

    this->editMenu_ = new QMenu("Edit", this);
    this->menuBar()->addMenu(this->editMenu_);
    this->editMenu_->addActions({
        this->actUndo_,
        this->actRedo_,
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
    });

    this->toolsMenu_ = new QMenu("Tools", this);
    this->menuBar()->addMenu(this->toolsMenu_);
    this->toolsMenu_->addActions({
        this->actOpenSettings_,
    });

    this->commandsMenu_ = new QMenu("Commands", this);
    this->toolsMenu_->addMenu(this->commandsMenu_);
    this->commandsMenu_->addActions({
        this->actExecCurrentLine_,
        this->actExecPreviousLine_,
        this->actExecNextLine_,
        this->actToggleMultiLine_,
    });
    this->commandsMenu_->addSeparator();
    this->commandsMenu_->addActions({
        this->actCommScan_,
        this->actToggleAutoCommScan_,
    });

    this->serverMenu_ = new QMenu("Server", this);
    this->toolsMenu_->addMenu(this->serverMenu_);
    this->serverMenu_->addActions({
        this->actStartServer_,
        this->actConnectToServer_,
        this->actDisconnectFromServer_,
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
        bool ok;
        /* TODO: dont always ask for it, have a setting for it and maybe ask for
         * first time in the session*/
        int msDelay = QInputDialog::getInt(this, "Minimum delay", "Delay (ms)",
                                           1000, 0, INT32_MAX, 100, &ok);
        if (!ok)
        {
            this->actToggleMultiLine_->setChecked(false);
            return;
        }

        this->actToggleMultiLine_->setText("Stop stepping");
        this->actToggleMultiLine_->setStatusTip(
            "Stop the currently active command stepping");
        this->actToggleMultiLine_->setIcon(
            this->style()->standardIcon(QStyle::SP_MediaPause));

        this->editor_->setReadOnly(true);

        emit this->previewWorker_->processCommands(
            this->editor_->toPlainText(),
            this->editor_->textCursor().blockNumber(), msDelay);
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

void MainWindow::onLineStarted(const int &lineNum)
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

void MainWindow::onCommandError(CommandErr err)
{
    emit this->messageController_->error(err, this);
}

void MainWindow::onConnectionError(ConnectionErr err)
{
    emit this->messageController_->error(err, this);
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

void MainWindow::onChangePreview(QByteArray data, const unsigned long &size)
{
    this->camPreview_->showPreview(data, size);
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
