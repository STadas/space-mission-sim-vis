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
    this->textEdit_ = new QTextEdit(this->centralWidget_);

    //NOTE: could probably make a pane/window system to make this more flexible
    this->centralLayout_->addWidget(this->textEdit_);
}

void MainWindow::createMenus()
{
    this->menuBar_ = new QMenuBar(this);

    this->fileMenu_ = new QMenu("File", this);
    this->menuBar_->addMenu(this->fileMenu_);
}

void MainWindow::createActions()
{
    this->actNewFile_ = new QAction("New file", this);
    this->actNewFile_->setShortcuts(QKeySequence::New);
    this->actNewFile_->setStatusTip("Start editing a new file");
    this->fileMenu_->addAction(actNewFile_);
    connect(this->actNewFile_, &QAction::triggered, this, &MainWindow::newFile);

    this->actOpenFile_ = new QAction("Open file", this);
    this->actOpenFile_->setShortcuts(QKeySequence::Open);
    this->actOpenFile_->setStatusTip("Open a file to edit");
    this->fileMenu_->addAction(actOpenFile_);
    connect(this->actOpenFile_, &QAction::triggered, this,
            &MainWindow::openFile);

    this->actSaveFileAs_ = new QAction("Save as", this);
    this->actSaveFileAs_->setShortcuts(QKeySequence::SaveAs);
    this->actSaveFileAs_->setStatusTip("Save as a new file");
    this->fileMenu_->addAction(actSaveFileAs_);
    connect(this->actSaveFileAs_, &QAction::triggered, this,
            &MainWindow::saveFileAs);

    this->actSaveFile_ = new QAction("Save", this);
    this->actSaveFile_->setShortcuts(QKeySequence::Save);
    this->actSaveFile_->setStatusTip("Save the currently opened file");
    this->fileMenu_->addAction(actSaveFile_);
    connect(this->actSaveFile_, &QAction::triggered, this,
            &MainWindow::saveFile);
}

void MainWindow::newFile()
{
    if (this->textEdit_->document()->isModified())
    {
        QMessageBox::StandardButton answer = QMessageBox::question(
            this, "New file",
            "Are you sure? This will discard any unsaved changes.");
        if (answer == QMessageBox::No)
        {
            return;
        }
    }

    this->textEdit_->setPlainText("");
}

void MainWindow::openFile()
{
    if (this->textEdit_->document()->isModified())
    {
        QMessageBox::StandardButton answer = QMessageBox::question(
            this, "Open file",
            "Are you sure? This will discard any unsaved changes.");
        if (answer == QMessageBox::No)
        {
            return;
        }
    }

    QString filePath = QFileDialog::getOpenFileName(
        this, "Open file", "", "Flight files (*.fli);;All files (*)");
    if (filePath.length() == 0)
    {
        return;
    }

    std::optional<QString> newText = FileUtil::readFile(filePath);
    if (!newText.has_value())
    {
        QMessageBox::critical(this, "Open file",
                              "There was an error opening the file.");
        return;
    }

    this->defaultSavePath_ = filePath;
    this->textEdit_->setPlainText(newText.value());
}

void MainWindow::saveFileAs()
{
    QString filePath = QFileDialog::getSaveFileName(this, "Save file as", "",
                                                    "Flight files (*.fli)");
    if (filePath.length() == 0)
    {
        return;
    }

    this->defaultSavePath_ = filePath;
    QFileDevice::FileError err =
        FileUtil::writeFile(this->textEdit_->toPlainText(), filePath);

    if (err)
    {
        QMessageBox::critical(this, "Save file as",
                              "There was an error saving the file.");
    }
}

void MainWindow::saveFile()
{
    if (this->defaultSavePath_ == "")
    {
        return this->saveFileAs();
    }

    QFileDevice::FileError err = FileUtil::writeFile(
        this->textEdit_->toPlainText(), this->defaultSavePath_);

    if (err)
    {
        QMessageBox::critical(this, "Save file",
                              "There was an error saving the file.");
    }
}
