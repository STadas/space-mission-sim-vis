#include "MainWindow.hpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->activeConnection = new PanguConnection("localhost", 10363);
    this->activeConnection->connect();

    this->createMenus();
    this->createActions();

    this->centralWidget_ = new QWidget(this);
    this->statusBar_ = new QStatusBar(this);

    this->setCentralWidget(this->centralWidget_);
    this->setStatusBar(this->statusBar_);
    this->setMenuBar(this->menuBar_);

    this->centralLayout_ = new VBoxLayout(this->centralWidget_);
    this->activeEditor_ = new Editor(this->centralWidget_);

    this->centralLayout_->addWidget(this->activeEditor_);

    this->activePreview_ = new SimPreview(this);

    /* TODO: refactor this to make it easier to add and remove other elements */
    QDockWidget *dock = new QDockWidget("Preview", this);
    dock->setWidget(this->activePreview_);
    this->addDockWidget(Qt::RightDockWidgetArea, dock);
}

void MainWindow::createMenus()
{
    this->menuBar_ = new QMenuBar(this);

    this->fileMenu_ = new QMenu("File", this);
    this->menuBar_->addMenu(this->fileMenu_);

    this->toolsMenu_ = new QMenu("Tools", this);
    this->menuBar_->addMenu(this->toolsMenu_);
}

/* TODO: move this into its own thing somewhere else */
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

    this->actLineExec_ = new QAction("Execute command", this);
    this->actLineExec_->setStatusTip(
        "Execute the command in the currently active line");
    this->toolsMenu_->addAction(this->actLineExec_);
    connect(this->actLineExec_, &QAction::triggered, this,
            &MainWindow::execActiveLine);
}

void MainWindow::newFile()
{
    if (this->activeEditor_->isModified())
    {
        QMessageBox::StandardButton answer =
            MessageController::fileMessage(FileMessage::FILE_NEW, this);
        if (answer == QMessageBox::No)
        {
            return;
        }
    }

    this->activeEditor_->clear();
}

void MainWindow::openFile()
{
    if (this->activeEditor_->isModified())
    {
        QMessageBox::StandardButton answer =
            MessageController::fileMessage(FileMessage::FILE_OPEN, this);
        if (answer == QMessageBox::No)
        {
            return;
        }
    }

    if (this->activeEditor_->load())
    {
        MessageController::fileMessage(FileMessage::OPEN_FAIL, this);
    };
}

void MainWindow::saveFileAs()
{
    if (this->activeEditor_->saveAs())
    {
        MessageController::fileMessage(FileMessage::SAVE_FAIL, this);
    }
}

void MainWindow::saveFile()
{
    if (this->activeEditor_->save())
    {
        MessageController::fileMessage(FileMessage::SAVE_FAIL, this);
    }
}

void MainWindow::execActiveLine()
{
    std::unique_ptr<ParsedCommand> parsedCommand;

    CommandErr commandErr = CommandUtil::parsePangu(
        this->activeEditor_->activeLineText(), parsedCommand);

    if (commandErr != CommandErr::OK)
    {
        MessageController::commandError(commandErr, this);
        return;
    }

    unsigned char *img = nullptr;

    size_t size{};
    ConnectionErr connectionErr =
        this->activeConnection->sendCommand(*parsedCommand, img, size);

    if (connectionErr != ConnectionErr::OK)
    {
        MessageController::connectionError(connectionErr, this);
        return;
    }

    if (!parsedCommand->expectsImage())
    {
        if (img != nullptr)
            delete img;
        return;
    }

    this->activePreview_->showPreview(img, size);
    delete img;
}
