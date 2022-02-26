#pragma once

#include <QtWidgets>

#include "interfaces/IEditor.hpp"
#include "util/FileUtil.hpp"

class PanguEditor : public QPlainTextEdit, IEditor
{
    Q_OBJECT

public:
    PanguEditor(QWidget *parent);
    ~PanguEditor() override;

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
