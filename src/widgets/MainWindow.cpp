#include "MainWindow.hpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , camPreview_(new CamPreview(this))
    , editor_(new Editor(this))
    , progressBar_(new ProgressBar(this))
    , autoCommScan_(false)
    , messageController_(new MessageController(this))
    , previewWorker_(new PreviewWorker)  // no parent so it can moveToThread
    , previewWorkerThread_(new QThread(this))
    , serverProcess_(new PanguServerProcess(this))
    , settings_(new Settings(this))
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

    this->createSignalConnections();
    this->createActions();
    this->createMenus();

    this->centralWidget()->setLayout(new VBoxLayout);
    this->centralWidget()->layout()->addWidget(this->editor_);

    //TODO: refactor this to make it easier to add and remove other elements
    QDockWidget *dockPreview = new QDockWidget("Simulation Preview", this);
    dockPreview->setWidget(this->camPreview_);
    this->addDockWidget(Qt::RightDockWidgetArea, dockPreview);

    QDockWidget *dockProgressBar = new QDockWidget("Simulation Progress", this);
    dockProgressBar->setWidget(this->progressBar_);
    this->addDockWidget(Qt::RightDockWidgetArea, dockProgressBar);
}

MainWindow::~MainWindow()
{
    this->previewWorkerThread_->quit();
    this->previewWorkerThread_->wait();
}

void MainWindow::createSignalConnections()
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
                     this, &MainWindow::onTextProcessed);

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

void MainWindow::createMenus()
{
    this->fileMenu_ = new QMenu("File", this);
    this->menuBar()->addMenu(this->fileMenu_);
    this->fileMenu_->addAction(actFileNew_);
    this->fileMenu_->addAction(actFileOpen_);
    this->fileMenu_->addAction(actFileSaveAs_);
    this->fileMenu_->addAction(actFileSave_);

    this->toolsMenu_ = new QMenu("Tools", this);
    this->menuBar()->addMenu(this->toolsMenu_);
    this->toolsMenu_->addAction(this->actOpenSettings_);

    this->commandsMenu_ = new QMenu("Commands", this);
    this->toolsMenu_->addMenu(this->commandsMenu_);
    this->commandsMenu_->addAction(this->actActiveLineExec_);
    this->commandsMenu_->addAction(this->actMultiLineStart_);
    this->commandsMenu_->addAction(this->actMultiLineStop_);
    this->commandsMenu_->addAction(this->actCommScan_);
    this->commandsMenu_->addAction(this->actToggleAutoCommScan_);

    this->serverMenu_ = new QMenu("Server", this);
    this->toolsMenu_->addMenu(this->serverMenu_);
    this->serverMenu_->addAction(this->actStartServer_);
    this->serverMenu_->addAction(this->actConnectToServer_);
    this->serverMenu_->addAction(this->actDisconnectFromServer_);
}

void MainWindow::createActions()
{
    this->actFileNew_ = new QAction("New file", this);
    this->actFileNew_->setShortcuts(QKeySequence::New);
    this->actFileNew_->setStatusTip("Start editing a new file");
    QObject::connect(this->actFileNew_, &QAction::triggered, this,
                     &MainWindow::onActFileNew);

    this->actFileOpen_ = new QAction("Open file", this);
    this->actFileOpen_->setShortcuts(QKeySequence::Open);
    this->actFileOpen_->setStatusTip("Open a file to edit");
    QObject::connect(this->actFileOpen_, &QAction::triggered, this,
                     &MainWindow::onActFileOpen);

    this->actFileSaveAs_ = new QAction("Save as", this);
    this->actFileSaveAs_->setShortcuts(QKeySequence::SaveAs);
    this->actFileSaveAs_->setStatusTip("Save as a new file");
    QObject::connect(this->actFileSaveAs_, &QAction::triggered, this,
                     &MainWindow::onActFileSaveAs);

    this->actFileSave_ = new QAction("Save", this);
    this->actFileSave_->setShortcuts(QKeySequence::Save);
    this->actFileSave_->setStatusTip("Save the currently opened file");
    QObject::connect(this->actFileSave_, &QAction::triggered, this,
                     &MainWindow::onActFileSave);

    this->actActiveLineExec_ = new QAction("Execute line", this);
    this->actActiveLineExec_->setStatusTip(
        "Execute the command in the currently active line");
    QObject::connect(this->actActiveLineExec_, &QAction::triggered, this,
                     &MainWindow::onActActiveLineExec);

    this->actMultiLineStart_ = new QAction("Step through lines", this);
    this->actMultiLineStart_->setStatusTip(
        "Step through and execute all commands (with delay) starting with the "
        "currently active line");
    QObject::connect(this->actMultiLineStart_, &QAction::triggered, this,
                     &MainWindow::onActMultiLineStart);

    this->actMultiLineStop_ = new QAction("Stop stepping", this);
    this->actMultiLineStop_->setStatusTip(
        "Stop the currently active command stepping");
    QObject::connect(this->actMultiLineStop_, &QAction::triggered, this,
                     &MainWindow::onActMultiLineStop);

    this->actCommScan_ = new QAction("Scan all commands", this);
    this->actCommScan_->setStatusTip("Scan all commands to update components "
                                     "like the simulation progress bar");
    QObject::connect(this->actCommScan_, &QAction::triggered, this,
                     &MainWindow::onActCommScan);

    this->actToggleAutoCommScan_ = new QAction("Auto command scanning", this);
    this->actToggleAutoCommScan_->setStatusTip(
        "Automatically scan all commands when editing to update components "
        "like the simulation progress bar");
    this->actToggleAutoCommScan_->setCheckable(true);
    this->actToggleAutoCommScan_->setChecked(false);
    QObject::connect(this->actToggleAutoCommScan_, &QAction::triggered, this,
                     &MainWindow::onActToggleAutoCommScan);

    this->actStartServer_ = new QAction("Start server", this);
    this->actStartServer_->setStatusTip("Start a server to send commands to");
    QObject::connect(this->actStartServer_, &QAction::triggered, this,
                     &MainWindow::onActStartServer);

    this->actConnectToServer_ = new QAction("Connect to server", this);
    this->actConnectToServer_->setStatusTip(
        "Connect to the currently used server");
    QObject::connect(this->actConnectToServer_, &QAction::triggered, this,
                     &MainWindow::onActConnectToServer);

    this->actDisconnectFromServer_ =
        new QAction("Disconnect from server", this);
    this->actDisconnectFromServer_->setStatusTip(
        "Disconnect from the currently used server without stopping it");
    QObject::connect(this->actDisconnectFromServer_, &QAction::triggered, this,
                     &MainWindow::onActDisconnectFromServer);

    this->actOpenSettings_ = new QAction("Settings", this);
    this->actOpenSettings_->setStatusTip("Open the settings window");
    QObject::connect(this->actOpenSettings_, &QAction::triggered, this,
                     &MainWindow::onActOpenSettings);
}

void MainWindow::onActFileNew()
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

void MainWindow::onActFileOpen()
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

void MainWindow::onActFileSaveAs()
{
    if (this->editor_->saveAs())
    {
        emit this->messageController_->error(FileErr::SAVE_FAIL, this);
    }
}

void MainWindow::onActFileSave()
{
    if (this->editor_->save())
    {
        emit this->messageController_->error(FileErr::SAVE_FAIL, this);
    }
}

void MainWindow::onActActiveLineExec()
{
    if (!this->previewWorker_->previewLock()->available())
        return;

    this->onLineStarted(this->editor_->textCursor().blockNumber());
    emit this->previewWorker_->processCommands(this->editor_->activeLineText());
}

void MainWindow::onActMultiLineStart()
{
    if (!this->previewWorker_->previewLock()->available())
        return;

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

void MainWindow::onActMultiLineStop()
{
    this->previewWorker_->cancelStepping();
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

void MainWindow::onActStopServer()
{
    emit this->previewWorker_->disconnect();
    this->serverProcess_->stop();
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
    SettingsDialog *settingsDialog = new SettingsDialog(this, this->settings_);
    settingsDialog->show();
}

void MainWindow::onLineStarted(const int &lineNum)
{
    this->editor_->goToLine(lineNum);

    if (this->progressBar_->value() != lineNum)
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

void MainWindow::onTextProcessed()
{
    this->editor_->setReadOnly(false);
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
