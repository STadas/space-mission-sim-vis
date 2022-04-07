#include "CommandsPage.hpp"

CommandsPage::CommandsPage(QWidget *parent, Settings *const settings)
    : SettingsPage(parent, settings)
{
    QGroupBox *steppingGroup = new QGroupBox("Stepping", this);
    QFormLayout *steppingLayout = new QFormLayout(steppingGroup);
    this->layout()->addWidget(steppingGroup);

    steppingLayout->addRow(
        "Stop stepping on unsupported command",
        this->createCheckBox(this->settings_->commandsStepStopUnimplemented));
}

CommandsPage::~CommandsPage()
{
}
