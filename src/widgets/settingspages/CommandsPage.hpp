#pragma once

#include <QtCore>
#include <QtWidgets>

#include "widgets/settingspages/SettingsPage.hpp"

class CommandsPage : public SettingsPage
{
public:
    CommandsPage(QWidget *parent, Settings *const settings);
    ~CommandsPage();
};
