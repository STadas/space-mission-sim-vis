#pragma once

#include <QtWidgets>

#include "interfaces/IEditor.hpp"
#include "util/FileUtil.hpp"

class Editor : public QPlainTextEdit, public IEditor
{
    Q_OBJECT

public:
    Editor(QWidget *parent);
    ~Editor() override;

    bool isModified() override;
    QString activeLineText() override;
    void goToLine(int lineNum) override;

    int save() override;
    int saveAs() override;
    int load() override;
    void clear() override;

    void keyPressEvent(QKeyEvent *keyEvent) override;

private:
    QString defaultSavePath_{};

    void highlightCurrentLine();
};
