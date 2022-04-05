#include "Editor.hpp"

Editor::Editor(QWidget *parent, Settings *const settings)
    : QPlainTextEdit(parent)
    , settings_(settings)
{
    this->setLineWrapMode(QPlainTextEdit::NoWrap);

    QObject::connect(this, &QPlainTextEdit::cursorPositionChanged, this, [=] {
        if (!this->isReadOnly())
        {
            this->highlightCurrentLine();
        }
    });

    this->setContextMenuPolicy(Qt::CustomContextMenu);
    QObject::connect(this, &QPlainTextEdit::customContextMenuRequested, this,
                     &Editor::showContextMenu);

    highlightCurrentLine();
}

Editor::~Editor()
{
}

bool Editor::isModified() const
{
    return this->document()->isModified();
}

QString Editor::activeLineText() const
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

void Editor::goToLine(int lineNum)
{
    QTextCursor newCursor(this->document()->findBlockByLineNumber(
        std::min(lineNum, this->document()->lineCount() - 1)));
    this->setTextCursor(newCursor);
    this->highlightCurrentLine();
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

    auto err = FileUtil::writeFile(this->defaultSavePath_, this->toPlainText());
    if (err != QFileDevice::FileError::NoError)
    {
        return err;
    }

    this->document()->setModified(false);

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

void Editor::showContextMenu(const QPoint &pt)
{
    QMenu *menu = this->createStandardContextMenu();
    menu->insertActions(menu->actions().at(0), this->extraContextMenuActions_);
    menu->exec(this->mapToGlobal(pt));
    menu->deleteLater();
}

void Editor::setExtraContextMenuActions(const QList<QAction *> &actions)
{
    this->extraContextMenuActions_ = actions;
}

void Editor::highlightCurrentLine()
{
    /* https://doc.qt.io/qt-5/qtwidgets-widgets-codeeditor-example.html */
    QList<QTextEdit::ExtraSelection> extraSelections;

    QTextEdit::ExtraSelection selection;

    QColor lineColor = QColor(this->settings_->currLineHighlightColor.value());

    selection.format.setBackground(lineColor);
    selection.format.setProperty(QTextFormat::FullWidthSelection, true);
    selection.cursor = this->textCursor();
    selection.cursor.clearSelection();
    extraSelections.append(selection);

    this->setExtraSelections(extraSelections);
}
