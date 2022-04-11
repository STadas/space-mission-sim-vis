#pragma once

#include <QtCore>
#include <QtWidgets>

#include "widgets/settingspages/SettingsPage.hpp"

/**
 * Editor settings page class
 */
class EditorPage : public SettingsPage
{
public:
    /**
     * Constructor
     * @param parent Parent widget
     * @param settings Settings object
     */
    EditorPage(QWidget *parent, Settings *const settings);
    /**
     * Destructor
     */
    ~EditorPage();
};
