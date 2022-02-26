#include "MainWindow.hpp"
#include "common/MessageController.hpp"
#include "common/PanguWorker.hpp"
#include "widgets/PanguEditor.hpp"
#include "widgets/SimPreview.hpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , editor_(new PanguEditor(this))
    , preview_(new SimPreview(this))
    , messageController_(new MessageController(this))
    , workerThread_(new QThread)
    , panguWorker_(new PanguWorker)
{
    /* As weird as this is, it needs to be done for us to be able to use the
     * enums with signals and slots. Potential for a generated source code
     * file? */
    qRegisterMetaType<CommandErr>("CommandErr");
    qRegisterMetaType<ConnectionErr>("ConnectionErr");
    qRegisterMetaType<FileErr>("FileErr");
    qRegisterMetaType<FileQuestion>("FileQuestion");

    this->panguWorker_->moveToThread(this->workerThread_);
    this->workerThread_->start();

    this->setCentralWidget(new QWidget(this));
    this->setStatusBar(new QStatusBar(this));
    this->setMenuBar(new QMenuBar(this));

    this->createSignalConnections();
    this->createMenus();
    this->createActions();

    this->centralWidget()->setLayout(new VBoxLayout);
    this->centralWidget()->layout()->addWidget(this->editor_);

    //TODO: refactor this to make it easier to add and remove other elements
    QDockWidget *dock = new QDockWidget("Preview", this);
    dock->setWidget(this->preview_);
    this->addDockWidget(Qt::RightDockWidgetArea, dock);
}

MainWindow::~MainWindow()
{
}

void MainWindow::createSignalConnections()
{
    connect(this->panguWorker_, qOverload<CommandErr>(&PanguWorker::error),
            this, [=](CommandErr err) {
                emit this->messageController_->error(err, this);
            });

    connect(this->panguWorker_, qOverload<ConnectionErr>(&PanguWorker::error),
            this, [=](ConnectionErr err) {
                emit this->messageController_->error(err, this);
            });

    connect(this->panguWorker_, &PanguWorker::askLine, this,
            [=](int fromLine, int toLine, int msDelay) {
                this->editor_->goToLine(fromLine);
                emit this->panguWorker_->giveLine(
                    this->editor_->activeLineText(), fromLine, toLine, msDelay);
            });

    connect(this->panguWorker_, &PanguWorker::changePreview, this->preview_,
            &SimPreview::changePreview);

    connect(this->panguWorker_, &PanguWorker::multiLineDone, this, [=] {
        this->panguWorker_->setCancelled(false);
        this->editor_->setReadOnly(false);
    });
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
        if (this->panguWorker_->previewLock->available())
            emit this->panguWorker_->giveLine(this->editor_->activeLineText());
    });

    this->actMultiLineStart_ = new QAction("Step through commands", this);
    this->actMultiLineStart_->setStatusTip(
        "Step through and execute all commands (with delay) starting with the "
        "currently active line");
    this->toolsMenu_->addAction(this->actMultiLineStart_);
    connect(this->actMultiLineStart_, &QAction::triggered, this, [=] {
        if (!this->panguWorker_->previewLock->available())
            return;

        bool ok;
        int msDelay = QInputDialog::getInt(this, "Command delay", "Delay (ms)",
                                           1000, 0, INT32_MAX, 100, &ok);
        if (!ok)
            return;

        this->editor_->setReadOnly(true);

        emit this->panguWorker_->doMultiLine(
            this->editor_->textCursor().blockNumber(),
            this->editor_->document()->lineCount(), msDelay);
    });

    this->actMultiLineStop_ = new QAction("Stop stepping", this);
    this->actMultiLineStop_->setStatusTip(
        "Stop the currently active command stepping");
    this->toolsMenu_->addAction(this->actMultiLineStop_);
    connect(this->actMultiLineStop_, &QAction::triggered, this->panguWorker_,
            &PanguWorker::stopMultiLine);
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
