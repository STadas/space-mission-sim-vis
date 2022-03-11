#include "ServerPage.hpp"

ServerPage::ServerPage(QWidget *parent, Settings *const settings)
    : SettingsPage(parent, settings)
{
    QFormLayout *form = new QFormLayout;
    form->setMargin(0);
    this->setLayout(form);
    form->addRow("Server executable path",
                 this->createLineEditBrowse(
                     this->settings_->serverPath, "Select executable", "",
                     "Executables (*.exe);;Bash scripts (*.sh);;Batch scripts "
                     "(*.bat);;All files (*)"));
    form->addRow("Server executable flags",
                 this->createLineEdit(this->settings_->serverFlags));
    form->addRow("Server address",
                 this->createLineEdit(this->settings_->serverAddress));
    form->addRow("Server port", this->createLineEdit(
                                    this->settings_->serverPort, 1025, 65535));
}

ServerPage::~ServerPage()
{
}
