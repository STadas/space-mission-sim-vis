#pragma once

#include <QtCore>
#include <QtWidgets>

#include "common/MessageController.hpp"
#include "common/settings/Settings.hpp"
#include "widgets/Editor.hpp"

class RecentsMenu : public QMenu
{
    Q_OBJECT

public:
    RecentsMenu(QWidget *parent, Settings *const settings,
                MessageController *const messageController);
    ~RecentsMenu();

    void updateActions(Editor *const editor);

private:
    Settings *const settings_;
    MessageController *const messageController_;

signals:
    void error(FileErr err);
};
