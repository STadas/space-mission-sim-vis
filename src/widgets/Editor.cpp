#include "Editor.hpp"

Editor::Editor(QWidget *parent)
    : QPlainTextEdit(parent)
{
}

Editor::~Editor()
{
}

bool Editor::isModified()
{
    return this->document()->isModified();
}

QString Editor::activeLineText()
{
    return this->textCursor().block().text();
}

int Editor::load()
{
    QFileDevice::FileError err = QFileDevice::FileError::NoError;

    QString filePath = QFileDialog::getOpenFileName(
        this, "Open file", "", "Flight files (*.fli);;All files (*)");
    if (filePath.length() == 0)
    {
        return err;
    }

    QString newText;
    err = FileUtil::readFile(filePath, newText);
    if (err != QFileDevice::FileError::NoError)
    {
        return err;
    }

    this->defaultSavePath_ = filePath;
    this->setPlainText(newText);

    return err;
}

void Editor::clear()
{
    this->setPlainText("");
}

int Editor::save()
{
    if (this->defaultSavePath_ == "")
    {
        return this->saveAs();
    }

    auto err = FileUtil::writeFile(this->toPlainText(), this->defaultSavePath_);
    if (err == QFileDevice::FileError::NoError)
    {
        this->document()->setModified(false);
    }
    return err;
}

int Editor::saveAs()
{
    QString filePath = QFileDialog::getSaveFileName(
        this, "Save file as", "", "Flight files (*.fli);;All files (*)");
    if (filePath.length() == 0)
    {
        return 0;
    }
    this->defaultSavePath_ = filePath;

    auto err = FileUtil::writeFile(filePath, this->toPlainText());
    if (err == QFileDevice::FileError::NoError)
    {
        this->document()->setModified(false);
    }
    return err;
}
