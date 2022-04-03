#pragma once

#include <QtCore>
#include <QtWidgets>

#include <float.h>

#include "widgets/settingspages/SettingsPage.hpp"

class CoordsVisPage : public SettingsPage
{
public:
    CoordsVisPage(QWidget *parent, Settings *const settings);
    ~CoordsVisPage();
};
