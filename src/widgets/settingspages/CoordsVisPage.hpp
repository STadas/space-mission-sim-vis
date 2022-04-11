#pragma once

#include <QtCore>
#include <QtWidgets>

#include <float.h>

#include "widgets/settingspages/SettingsPage.hpp"

/**
 * Coords vis settings page class
 */
class CoordsVisPage : public SettingsPage
{
public:
    /**
     * Constructor
     * @param parent Parent widget
     * @param settings Settings object
     */
    CoordsVisPage(QWidget *parent, Settings *const settings);
    /**
     * Destructor
     */
    ~CoordsVisPage();
};
