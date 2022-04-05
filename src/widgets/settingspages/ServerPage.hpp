#pragma once

#include <QtCore>
#include <QtWidgets>

#include "common/VBoxLayout.hpp"
#include "widgets/settingspages/SettingsPage.hpp"

class ServerPage : public SettingsPage
{
public:
    ServerPage(QWidget *parent, Settings *const settings);
    ~ServerPage();
};
