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

/**
 * Settings dialog class
 */
class SettingsDialog : public Dialog
{
    Q_OBJECT

public:
    /**
     * Constructor
     * @param parent Parent widget
     * @param settings Settings object
     * @param resources Resources object
     */
    SettingsDialog(QWidget *parent, Settings *const settings,
                   Resources *const resources);
    /**
     * Destructor
     */
    ~SettingsDialog();

private slots:
    void onCurrentChanged(const int &current);
    void onOkClicked();
    void onCancelClicked();

private:
    /**
     * Save the window's geometry
     */
    void save() override;
    /**
     *  Load the windows's geometry
     */
    void load() override;

    /**
     * Create settings pages
     */
    void createPages();
    /**
     * Add a settings page
     * @param action Action with an icon and text
     * @param page Settings page widget
     */
    void addPage(QAction *action, SettingsPage *page);

    QListWidget *pageSelect_;
    QStackedWidget *pages_;

    QPushButton *okButton;
    QPushButton *cancelButton;

    Settings *const settings_;
    Resources *const resources_;
};
