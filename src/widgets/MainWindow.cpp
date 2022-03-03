#include "MainWindow.hpp"
#include "common/PreviewWorker.hpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , preview_(new CamPreview(this))
    , editor_(new Editor(this))
    , progressBar_(new ProgressBar(this))
    , autoCommScan_(false)
    , messageController_(new MessageController(this))
    , previewWorker_(new PreviewWorker)
    , previewWorkerThread_(new QThread)
{
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
    this->createMenus();
    this->createActions();

    this->centralWidget()->setLayout(new VBoxLayout);
    this->centralWidget()->layout()->addWidget(this->editor_);

    //TODO: refactor this to make it easier to add and remove other elements
    QDockWidget *dockPreview = new QDockWidget("Simulation Preview", this);
    dockPreview->setWidget(this->preview_);
    this->addDockWidget(Qt::RightDockWidgetArea, dockPreview);

    QDockWidget *dockProgressBar = new QDockWidget("Simulation Progress", this);
    dockProgressBar->setWidget(this->progressBar_);
    this->addDockWidget(Qt::RightDockWidgetArea, dockProgressBar);
}

MainWindow::~MainWindow()
{
}

void MainWindow::createSignalConnections()
{
    auto onCommandError = [=](CommandErr err) {
        emit this->messageController_->error(err, this);
    };
    connect(this->previewWorker_, qOverload<CommandErr>(&PreviewWorker::error),
            this, onCommandError);

    auto onConnectionError = [=](ConnectionErr err) {
        emit this->messageController_->error(err, this);
    };
    connect(this->previewWorker_,
            qOverload<ConnectionErr>(&PreviewWorker::error), this,
            onConnectionError);

    auto onAskLine = [=](int fromLine, int toLine, int msDelay) {
        this->editor_->goToLine(fromLine);
        emit this->previewWorker_->giveLine(this->editor_->activeLineText(),
                                            fromLine, toLine, msDelay);

        if (this->progressBar_->value() != fromLine)
        {
            auto indices = this->previewWorker_->imgIndices();
            auto it = std::find(indices.begin(), indices.end(), fromLine);
            if (it != indices.end())
            {
                this->progressBar_->blockSignals(true);
                this->progressBar_->setValue(it - indices.begin());
                this->progressBar_->blockSignals(false);
            }
        }
    };
    connect(this->previewWorker_, &PreviewWorker::askLine, this, onAskLine);

    connect(this->previewWorker_, &PreviewWorker::changePreview, this->preview_,
            &CamPreview::showPreview);

    auto onMultiLineDone = [=] {
        this->editor_->setReadOnly(false);
    };
    connect(this->previewWorker_, &PreviewWorker::multiLineDone, this,
            onMultiLineDone);

    auto onPBarChanged = [=](int imgIdx) {
        this->previewWorker_->setCancelled(true);
        if (this->previewWorker_->previewLock()->available())
        {
            emit this->previewWorker_->askLine(
                this->previewWorker_->imgIndices()[imgIdx]);
        }
        else
        {
            this->editor_->goToLine(this->previewWorker_->imgIndices()[imgIdx]);
        }
    };
    connect(this->progressBar_, &ProgressBar::valueChanged, this,
            onPBarChanged);

    auto onPBarReleased = [=] {
        this->previewWorker_->setCancelled(true);
        emit this->previewWorker_->askLine(
            this->previewWorker_->imgIndices()[this->progressBar_->value()]);
    };
    connect(this->progressBar_, &ProgressBar::sliderReleased, this,
            onPBarReleased);

    auto onEditorContentChanged = [=] {
        if (autoCommScan_)
        {
            emit this->previewWorker_->updateImgIndices(
                this->editor_->toPlainText());
        }
    };
    connect(this->editor_->document(), &QTextDocument::contentsChanged, this,
            onEditorContentChanged);

    auto onImgIndicesUpdated = [=] {
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
    };
    connect(this->previewWorker_, &PreviewWorker::imgIndicesUpdated, this,
            onImgIndicesUpdated);
}

void MainWindow::createMenus()
{
    this->fileMenu_ = new QMenu("File", this);
    this->menuBar()->addMenu(this->fileMenu_);

    this->toolsMenu_ = new QMenu("Tools", this);
    this->menuBar()->addMenu(this->toolsMenu_);
}

void MainWindow::createActions()
{
    this->actFileNew_ = new QAction("New file", this);
    this->actFileNew_->setShortcuts(QKeySequence::New);
    this->actFileNew_->setStatusTip("Start editing a new file");
    this->fileMenu_->addAction(actFileNew_);
    connect(this->actFileNew_, &QAction::triggered, this, &MainWindow::newFile);

    this->actFileOpen_ = new QAction("Open file", this);
    this->actFileOpen_->setShortcuts(QKeySequence::Open);
    this->actFileOpen_->setStatusTip("Open a file to edit");
    this->fileMenu_->addAction(actFileOpen_);
    connect(this->actFileOpen_, &QAction::triggered, this,
            &MainWindow::openFile);

    this->actFileSaveAs_ = new QAction("Save as", this);
    this->actFileSaveAs_->setShortcuts(QKeySequence::SaveAs);
    this->actFileSaveAs_->setStatusTip("Save as a new file");
    this->fileMenu_->addAction(actFileSaveAs_);
    connect(this->actFileSaveAs_, &QAction::triggered, this,
            &MainWindow::saveFileAs);

    this->actFileSave_ = new QAction("Save", this);
    this->actFileSave_->setShortcuts(QKeySequence::Save);
    this->actFileSave_->setStatusTip("Save the currently opened file");
    this->fileMenu_->addAction(actFileSave_);
    connect(this->actFileSave_, &QAction::triggered, this,
            &MainWindow::saveFile);

    this->actActiveLineExec_ = new QAction("Execute command", this);
    this->actActiveLineExec_->setStatusTip(
        "Execute the command in the currently active line");
    this->toolsMenu_->addAction(this->actActiveLineExec_);
    connect(this->actActiveLineExec_, &QAction::triggered, this, [=] {
        if (this->previewWorker_->previewLock()->available())
        {
            emit this->previewWorker_->giveLine(
                this->editor_->activeLineText());
        }
    });

    this->actMultiLineStart_ = new QAction("Step through commands", this);
    this->actMultiLineStart_->setStatusTip(
        "Step through and execute all commands (with delay) starting with the "
        "currently active line");
    this->toolsMenu_->addAction(this->actMultiLineStart_);
    connect(this->actMultiLineStart_, &QAction::triggered, this, [=] {
        if (!this->previewWorker_->previewLock()->available())
            return;

        bool ok;
        int msDelay = QInputDialog::getInt(this, "Command delay", "Delay (ms)",
                                           1000, 0, INT32_MAX, 100, &ok);
        if (!ok)
            return;

        this->editor_->setReadOnly(true);

        emit this->previewWorker_->doMultiLine(
            this->editor_->textCursor().blockNumber(),
            this->editor_->document()->lineCount(), msDelay);
    });

    this->actMultiLineStop_ = new QAction("Stop stepping", this);
    this->actMultiLineStop_->setStatusTip(
        "Stop the currently active command stepping");
    this->toolsMenu_->addAction(this->actMultiLineStop_);
    connect(this->actMultiLineStop_, &QAction::triggered, this->previewWorker_,
            &PreviewWorker::stopMultiLine);

    this->actCommScan = new QAction("Scan all commands", this);
    this->actCommScan->setStatusTip("Scan all commands to update components "
                                    "like the simulation progress bar");
    this->toolsMenu_->addAction(this->actCommScan);
    connect(this->actCommScan, &QAction::triggered, this, [=] {
        emit this->previewWorker_->updateImgIndices(
            this->editor_->toPlainText());
    });

    this->actToggleAutoCommScan_ = new QAction("Auto command scanning", this);
    this->actToggleAutoCommScan_->setStatusTip(
        "Automatically scan all commands when editing to update components "
        "like the simulation progress bar");
    this->actToggleAutoCommScan_->setCheckable(true);
    this->actToggleAutoCommScan_->setChecked(false);
    this->toolsMenu_->addAction(this->actToggleAutoCommScan_);
    connect(this->actToggleAutoCommScan_, &QAction::triggered, this, [=] {
        this->autoCommScan_ = this->actToggleAutoCommScan_->isChecked();
        if (this->actToggleAutoCommScan_->isChecked())
        {
            emit this->previewWorker_->updateImgIndices(
                this->editor_->toPlainText());
        }
    });
}

void MainWindow::newFile()
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

void MainWindow::openFile()
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

void MainWindow::saveFileAs()
{
    if (this->editor_->saveAs())
    {
        emit this->messageController_->error(FileErr::SAVE_FAIL, this);
    }
}

void MainWindow::saveFile()
{
    if (this->editor_->save())
    {
        emit this->messageController_->error(FileErr::SAVE_FAIL, this);
    }
}
