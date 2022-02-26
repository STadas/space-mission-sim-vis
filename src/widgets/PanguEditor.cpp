#include "PanguEditor.hpp"

PanguEditor::PanguEditor(QWidget *parent)
    : QPlainTextEdit(parent)
{
    this->setLineWrapMode(QPlainTextEdit::NoWrap);

    connect(this, &QPlainTextEdit::cursorPositionChanged, this, [=] {
        if (!this->isReadOnly())
        {
            this->highlightCurrentLine();
        }
    });

    highlightCurrentLine();
}

PanguEditor::~PanguEditor()
{
}

bool PanguEditor::isModified()
{
    return this->document()->isModified();
}

QString PanguEditor::activeLineText()
{
    return this->textCursor().block().text();
}

int PanguEditor::load()
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

void PanguEditor::goToLine(int lineNum)
{
    QTextCursor newCursor(this->document()->findBlockByLineNumber(
        std::min(lineNum, this->document()->lineCount() - 1)));
    this->setTextCursor(newCursor);
    this->highlightCurrentLine();
}

void PanguEditor::clear()
{
    this->setPlainText("");
}

int PanguEditor::save()
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

int PanguEditor::saveAs()
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

void PanguEditor::keyPressEvent(QKeyEvent *keyEvent)
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

void PanguEditor::highlightCurrentLine()
{
    /* https://doc.qt.io/qt-5/qtwidgets-widgets-codeeditor-example.html */
    QList<QTextEdit::ExtraSelection> extraSelections;

    QTextEdit::ExtraSelection selection;

    //TODO: could have a setting for the color
    QColor lineColor = QColor(Qt::yellow).lighter(128);

    selection.format.setBackground(lineColor);
    selection.format.setProperty(QTextFormat::FullWidthSelection, true);
    selection.cursor = this->textCursor();
    selection.cursor.clearSelection();
    extraSelections.append(selection);

    this->setExtraSelections(extraSelections);
}
