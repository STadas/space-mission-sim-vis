#include "Editor.hpp"

Editor::Editor(QWidget *parent)
    : QTextEdit(parent)
{
}

Editor::~Editor()
{
}

bool Editor::isModified()
{
    return this->document()->isModified();
}

int Editor::load()
{
    QString filePath = QFileDialog::getOpenFileName(
        this, "Open file", "", "Flight files (*.fli);;All files (*)");
    if (filePath.length() == 0)
    {
        return 0;
    }

    int err;
    QString newText = FileUtil::readFile(filePath, err);
    if (err)
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

    int err = FileUtil::writeFile(this->toPlainText(), this->defaultSavePath_);
    if (!err)
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

    int err = FileUtil::writeFile(this->toPlainText(), filePath);
    if (!err)
    {
        this->document()->setModified(false);
    }
    return err;
}
