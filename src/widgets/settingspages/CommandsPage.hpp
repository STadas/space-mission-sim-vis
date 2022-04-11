#pragma once

#include <QtCore>
#include <QtWidgets>

#include "widgets/settingspages/SettingsPage.hpp"

/**
 * Commands settings page class
 */
class CommandsPage : public SettingsPage
{
public:
    /**
     * Constructor
     * @param parent Parent widget
     * @param settings Settings object
     */
    CommandsPage(QWidget *parent, Settings *const settings);
    /**
     * Destructor
     */
    ~CommandsPage();
};
