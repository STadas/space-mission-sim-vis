#include "MainWindow.hpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , camPreview_(new CamPreview(this))
    , editor_(new Editor(this))
    , playbackInterface_(new PlaybackInterface(this))
    , progressBar_(this->playbackInterface_->progressBar_)
    , autoCommScan_(false)
    , messageController_(new MessageController(this))
    , previewWorker_(new PreviewWorker)  // no parent so it can moveToThread
    , previewWorkerThread_(new QThread(this))
    , serverProcess_(new PanguServerProcess(this))
    , settings_(new Settings(this))
    , resources_(new Resources(this))
{
    this->settings_->load();

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

    //TODO: refactor this to make it easier to add and remove other elements
    QDockWidget *dockCamPreview = new QDockWidget("Camera Preview", this);
    dockCamPreview->setObjectName("dockCamPreview");
    dockCamPreview->setWidget(this->camPreview_);
    this->addDockWidget(Qt::RightDockWidgetArea, dockCamPreview);

    QDockWidget *dockPlaybackControls =
        new QDockWidget("Playback Controls", this);
    dockPlaybackControls->setObjectName("dockPlaybackControls");
    dockPlaybackControls->setWidget(this->playbackInterface_);
    this->addDockWidget(Qt::RightDockWidgetArea, dockPlaybackControls);

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

    QObject::connect(this->previewWorker_, &PreviewWorker::imgIndicesUpdated,
                     this, &MainWindow::onImgIndicesUpdated);

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
    QObject::connect(this->actToggleMultiLine_, &QAction::triggered, this,
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
        "components like the playback progress bar");
    this->actToggleAutoCommScan_->setCheckable(true);
    this->actToggleAutoCommScan_->setChecked(false);
    this->actToggleAutoCommScan_->setIcon(this->resources_->iconRecurring);
    QObject::connect(this->actToggleAutoCommScan_, &QAction::triggered, this,
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
}

void MainWindow::initMenus()
{
    this->fileMenu_ = new QMenu("File", this);
    this->menuBar()->addMenu(this->fileMenu_);
    this->fileMenu_->addAction(actNewFile_);
    this->fileMenu_->addAction(actOpenFile_);
    this->fileMenu_->addAction(actSaveFile_);
    this->fileMenu_->addAction(actSaveFileAs_);

    this->toolsMenu_ = new QMenu("Tools", this);
    this->menuBar()->addMenu(this->toolsMenu_);
    this->toolsMenu_->addAction(this->actOpenSettings_);

    this->commandsMenu_ = new QMenu("Commands", this);
    this->toolsMenu_->addMenu(this->commandsMenu_);
    this->commandsMenu_->addAction(this->actExecCurrentLine_);
    this->commandsMenu_->addAction(this->actExecPreviousLine_);
    this->commandsMenu_->addAction(this->actExecNextLine_);
    this->commandsMenu_->addAction(this->actToggleMultiLine_);
    this->commandsMenu_->addAction(this->actCommScan_);
    this->commandsMenu_->addAction(this->actToggleAutoCommScan_);

    this->serverMenu_ = new QMenu("Server", this);
    this->toolsMenu_->addMenu(this->serverMenu_);
    this->serverMenu_->addAction(this->actStartServer_);
    this->serverMenu_->addAction(this->actConnectToServer_);
    this->serverMenu_->addAction(this->actDisconnectFromServer_);
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
            this->messageController_->question(FileQuestion::FILE_NEW, this);
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
            this->messageController_->question(FileQuestion::FILE_OPEN, this);
        if (answer == QMessageBox::No)
        {
            return;
        }
    }

    if (this->editor_->load())
    {
        emit this->messageController_->error(FileErr::OPEN_FAIL, this);
    };
}

void MainWindow::onActSaveFileAs()
{
    if (this->editor_->saveAs())
    {
        emit this->messageController_->error(FileErr::SAVE_FAIL, this);
    }
}

void MainWindow::onActSaveFile()
{
    if (this->editor_->save())
    {
        emit this->messageController_->error(FileErr::SAVE_FAIL, this);
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

void MainWindow::onActToggleMultiLine()
{
    if (!this->previewWorker_->previewLock()->available())
    {
        this->previewWorker_->cancelStepping();
        return;
    }

    this->actToggleMultiLine_->setText("Stop stepping");
    this->actToggleMultiLine_->setStatusTip(
        "Stop the currently active command stepping");
    this->actToggleMultiLine_->setIcon(
        this->style()->standardIcon(QStyle::SP_MediaPause));

    bool ok;
    int msDelay = QInputDialog::getInt(this, "Minimum delay", "Delay (ms)",
                                       1000, 0, INT32_MAX, 100, &ok);
    if (!ok)
        return;

    this->editor_->setReadOnly(true);

    emit this->previewWorker_->processCommands(
        this->editor_->toPlainText(), this->editor_->textCursor().blockNumber(),
        msDelay);
}

void MainWindow::onActCommScan()
{
    emit this->previewWorker_->updateImgIndices(this->editor_->toPlainText());
}

void MainWindow::onActToggleAutoCommScan()
{
    this->autoCommScan_ = this->actToggleAutoCommScan_->isChecked();
    if (this->actToggleAutoCommScan_->isChecked())
    {
        emit this->previewWorker_->updateImgIndices(
            this->editor_->toPlainText());
    }
}

void MainWindow::onActStartServer()
{
    this->serverProcess_->start(
        this->settings_->serverPath.value(),
        StringUtil::split(this->settings_->serverFlags.value()));
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

void MainWindow::onLineStarted(const int &lineNum)
{
    this->editor_->goToLine(lineNum);

    /* progress bar stuff */
    if (this->previewWorker_->imgIndices().size() == 0)
        return;

    int currPBarLineNum =
        this->previewWorker_->imgIndices()[this->progressBar_->value()];
    if (currPBarLineNum != lineNum)
    {
        auto indices = this->previewWorker_->imgIndices();
        auto it = std::find(indices.begin(), indices.end(), lineNum);
        if (it != indices.end())
        {
            this->progressBar_->blockSignals(true);
            this->progressBar_->setValue(it - indices.begin());
            this->progressBar_->blockSignals(false);
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
}

void MainWindow::onChangePreview(QByteArray data, const unsigned long &size)
{
    this->camPreview_->showPreview(data, size);
}

void MainWindow::onPBarChanged(int imgIndex)
{
    this->previewWorker_->cancelStepping();

    if (this->previewWorker_->previewLock()->available())
    {
        this->editor_->goToLine(this->previewWorker_->imgIndices()[imgIndex]);
        emit this->previewWorker_->processCommands(
            this->editor_->activeLineText());
    }
    else
    {
        this->editor_->goToLine(this->previewWorker_->imgIndices()[imgIndex]);
    }
}

void MainWindow::onPBarReleased()
{
    this->previewWorker_->cancelStepping();

    this->editor_->goToLine(
        this->previewWorker_->imgIndices()[this->progressBar_->value()]);
    emit this->previewWorker_->processCommands(this->editor_->activeLineText());
}

void MainWindow::onImgIndicesUpdated()
{
    if (this->previewWorker_->imgIndices().size() > 0)
    {
        this->progressBar_->setEnabled(true);
        this->progressBar_->setMaximum(
            this->previewWorker_->imgIndices().size() - 1);
    }
    else
    {
        this->progressBar_->setMaximum(0);
        this->progressBar_->setEnabled(false);
    }
}

void MainWindow::onEditorContentChanged()
{
    if (autoCommScan_)
        emit this->previewWorker_->updateImgIndices(
            this->editor_->toPlainText());
}
