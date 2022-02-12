#include "MainWindow.hpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->createMenus();
    this->createActions();

    this->centralWidget_ = new QWidget(this);
    this->statusBar_ = new QStatusBar(this);

    this->setCentralWidget(this->centralWidget_);
    this->setStatusBar(this->statusBar_);
    this->setMenuBar(this->menuBar_);

    this->centralLayout_ = new VBoxLayout(this->centralWidget_);
    this->activeEditor_ = new Editor(this->centralWidget_);

    //NOTE: could probably make a pane/window system to make this more flexible
    this->centralLayout_->addWidget(this->activeEditor_);
}

void MainWindow::createMenus()
{
    this->menuBar_ = new QMenuBar(this);

    this->fileMenu_ = new QMenu("File", this);
    this->menuBar_->addMenu(this->fileMenu_);
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
}

void MainWindow::newFile()
{
    if (this->activeEditor_->isModified())
    {
        QMessageBox::StandardButton answer = QMessageBox::question(
            this, "New file",
            "Are you sure? This will discard any unsaved changes.");
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
        QMessageBox::StandardButton answer = QMessageBox::question(
            this, "Open file",
            "Are you sure? This will discard any unsaved changes.");
        if (answer == QMessageBox::No)
        {
            return;
        }
    }

    if (this->activeEditor_->load())
    {
        QMessageBox::critical(this, "Open file",
                              "There was an error opening the file.");
    };
}

void MainWindow::saveFileAs()
{
    if (this->activeEditor_->saveAs())
    {
        QMessageBox::critical(this, "Save file as",
                              "There was an error saving the file.");
    }
}

void MainWindow::saveFile()
{
    if (this->activeEditor_->save())
    {
        QMessageBox::critical(this, "Save file",
                              "There was an error saving the file.");
    }
}
