#pragma once

#include <QtWidgets>
#include <string>

#include "interfaces/IEditor.hpp"
#include "util/FileUtil.hpp"

class Editor : public QPlainTextEdit, IEditor
{
    Q_OBJECT

public:
    Editor(QWidget *parent);
    ~Editor() override;

    bool isModified() override;
    QString activeLineText() override;

    int save() override;
    int saveAs() override;
    int load() override;
    void clear() override;

private:
    QString defaultSavePath_{};
};
