#pragma once

#include <QtWidgets>

#include "common/settings/Settings.hpp"
#include "interfaces/IEditor.hpp"
#include "util/FileUtil.hpp"

class Editor : public QPlainTextEdit, public IEditor
{
    Q_OBJECT

public:
    Editor(QWidget *parent, Settings *const settings);
    ~Editor() override;

    bool isModified() const override;
    QString activeLineText() const override;
    void goToLine(int lineNum) override;

    int save() override;
    int saveAs() override;
    int load() override;
    void clear() override;

    void keyPressEvent(QKeyEvent *keyEvent) override;
    void showContextMenu(const QPoint &pt);

    void setExtraContextMenuActions(const QList<QAction *> &actions);

private:
    QString defaultSavePath_{};
    Settings *const settings_;
    QList<QAction *> extraContextMenuActions_;

    void highlightCurrentLine();
};
