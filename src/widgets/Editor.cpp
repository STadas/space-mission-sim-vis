#include "Editor.hpp"

Editor::Editor(QWidget *parent)
    : QPlainTextEdit(parent)
{
    /* Set up active block highlighting */
    this->setLineWrapMode(QPlainTextEdit::NoWrap);
    QTextBlockFormat activeLineFmt = this->textCursor().blockFormat();
    QTextBlockFormat defaultLineFmt = this->textCursor().blockFormat();

    activeLineFmt.setBackground(Qt::lightGray);
    this->textCursor().setBlockFormat(activeLineFmt);

    connect(this, &QPlainTextEdit::cursorPositionChanged, this, [=]() {
        if (this->textCursor().block() != this->activeBlock_)
        {
            QTextCursor tempCrs = this->textCursor();

            tempCrs.select(QTextCursor::BlockUnderCursor);
            this->activeBlock_ = tempCrs.block();

            tempCrs.select(QTextCursor::Document);
            tempCrs.setBlockFormat(defaultLineFmt);

            tempCrs = QTextCursor(this->activeBlock_);
            tempCrs.setBlockFormat(activeLineFmt);
        }
    });
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
    return this->activeBlock_.text();
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

void Editor::keyPressEvent(QKeyEvent *keyEvent)
{
    /* We don't want newlines in blocks. Remove ShiftModifier flag to instead
     * make a new block when user presses Shift+Return */
    if ((keyEvent->modifiers() & Qt::ShiftModifier) &&
        keyEvent->key() == Qt::Key_Return)
    {
        keyEvent->setModifiers(keyEvent->modifiers() & ~Qt::ShiftModifier);
    }

    QPlainTextEdit::keyPressEvent(keyEvent);
}
