#pragma once

#include <QtCore>
#include <QtWidgets>

#include "common/HBoxLayout.hpp"
#include "common/Resources.hpp"
#include "common/VBoxLayout.hpp"
#include "common/settings/Settings.hpp"

#include "widgets/dialogs/Dialog.hpp"
#include "widgets/settingspages/CommandsPage.hpp"
#include "widgets/settingspages/CoordsVisPage.hpp"
#include "widgets/settingspages/EditorPage.hpp"
#include "widgets/settingspages/ServerPage.hpp"

class SettingsDialog : public Dialog
{
    Q_OBJECT

public:
    SettingsDialog(QWidget *parent, Settings *const settings,
                   Resources *const resources);
    ~SettingsDialog();

private:
    QListWidget *pageSelect_;
    QStackedWidget *pages_;

    QPushButton *okButton;
    QPushButton *cancelButton;

    Settings *const settings_;
    Resources *const resources_;

    void save() override;
    void load() override;

    void createPages();
    void addPage(QAction *action, QWidget *page);

private slots:
    void onCurrentChanged(const int &current);
    void onOkClicked();
    void onCancelClicked();
};
