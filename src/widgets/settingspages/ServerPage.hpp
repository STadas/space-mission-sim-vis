#pragma once

#include <QtCore>
#include <QtWidgets>

#include "widgets/settingspages/SettingsPage.hpp"

/**
 * Server settings page class
 */
class ServerPage : public SettingsPage
{
public:
    /**
     * Constructor
     * @param parent Parent widget
     * @param settings Settings object
     */
    ServerPage(QWidget *parent, Settings *const settings);
    /**
     * Destructor
     */
    ~ServerPage();
};
