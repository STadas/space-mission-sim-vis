#pragma once

#include <QtCore>
#include <QtWidgets>

#include "widgets/settingspages/SettingsPage.hpp"

class EditorPage : public SettingsPage
{
public:
    EditorPage(QWidget *parent, Settings *const settings);
    ~EditorPage();
};
