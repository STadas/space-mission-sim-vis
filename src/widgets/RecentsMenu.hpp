#pragma once

#include <QtCore>
#include <QtWidgets>

#include "common/MessageController.hpp"
#include "common/settings/Settings.hpp"
#include "widgets/Editor.hpp"

/**
 * Recents menu class
 */
class RecentsMenu : public QMenu
{
    Q_OBJECT

public:
    /**
     * Constructor
     * @param parent Parent widget
     * @param settings Settings object
     * @param messageController MessageController object
     */
    RecentsMenu(QWidget *parent, Settings *const settings,
                MessageController *const messageController);
    /**
     * Destructor
     */
    ~RecentsMenu();

    /**
     * Update the recents menu with new actions using the list in the settings
     * @param editor Editor to connect to when making the new actions
     */
    void updateActions(Editor *const editor);

signals:
    /**
     * Emitted on file error
     * @param error Error status
     */
    void error(FileErr err);

private:
    Settings *const settings_;
    MessageController *const messageController_;
};
