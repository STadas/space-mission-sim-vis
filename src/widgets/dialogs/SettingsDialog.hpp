#pragma once

#include <QtCore>
#include <QtWidgets>

#include "common/HBoxLayout.hpp"
#include "common/VBoxLayout.hpp"
#include "common/settings/Settings.hpp"

#include "widgets/settingspages/ServerPage.hpp"

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    SettingsDialog(QWidget *parent, Settings *const settings);

private:
    QListWidget *pageSelect_;
    QStackedWidget *pages_;

    QPushButton *okButton;
    QPushButton *cancelButton;

    Settings *const settings_;

    void createPages();
    void addPage(QAction *action, QWidget *page);

private slots:
    void onCurrentChanged(const int &current);
    void onOkClicked();
    void onCancelClicked();
};
