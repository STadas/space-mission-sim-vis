#pragma once

#include <QtWidgets>
#include <string>

#include "interfaces/IDocument.hpp"
#include "util/FileUtil.hpp"

class Editor : public QTextEdit, IDocument
{
    Q_OBJECT

public:
    Editor(QWidget *parent);
    ~Editor();

    bool isModified();

    int save();
    int saveAs();
    int load();
    void clear();

private:
    QString defaultSavePath_{};
};
