#include "ServerPage.hpp"

ServerPage::ServerPage(QWidget *parent, Settings *const settings)
    : SettingsPage(parent, settings)
{
    QGroupBox *execGroup = new QGroupBox("Executable", this);
    QFormLayout *execLayout = new QFormLayout(execGroup);
    this->layout()->addWidget(execGroup);

    execLayout->addRow(
        "Server executable path",
        this->createLineEditBrowse(
            this->settings_->serverPath, "Select executable", "",
            "Executables (*.exe);;Bash scripts (*.sh);;Batch scripts "
            "(*.bat);;All files (*)"));

    execLayout->addRow("PANGU .ini path",
                       this->createLineEditBrowse(
                           this->settings_->serverIni, "Select .ini file", "",
                           "INI files (*.ini);;All files (*)"));

    execLayout->addRow("Server executable flags",
                       this->createLineEdit(this->settings_->serverFlags));

    QGroupBox *connectionGroup = new QGroupBox("Connection", this);
    QFormLayout *connectionLayout = new QFormLayout(connectionGroup);
    this->layout()->addWidget(connectionGroup);

    connectionLayout->addRow(
        "Server address", this->createLineEdit(this->settings_->serverAddress));

    connectionLayout->addRow(
        "Server port",
        this->createLineEdit(this->settings_->serverPort, 1025, 65535));

    QGroupBox *cameraGroup = new QGroupBox("Camera preview", this);
    QFormLayout *cameraLayout = new QFormLayout(cameraGroup);
    this->layout()->addWidget(cameraGroup);

    QCheckBox *camPredefinedCBox =
        this->createCheckBox(this->settings_->serverCamPredefined);
    cameraLayout->addRow("Custom camera resolution", camPredefinedCBox);

    //TODO: make it so we can connect some sort of setting valueChanged signal
    QWidget *camWidthInput =
        this->createLineEdit(this->settings_->serverCamWidth, 0);
    camWidthInput->setEnabled(camPredefinedCBox->isChecked());
    cameraLayout->addRow("Camera resolution width", camWidthInput);
    QObject::connect(camPredefinedCBox, &QCheckBox::toggled, camWidthInput,
                     &QWidget::setEnabled);

    QWidget *camHeightInput =
        this->createLineEdit(this->settings_->serverCamHeight, 0);
    camHeightInput->setEnabled(camPredefinedCBox->isChecked());
    cameraLayout->addRow("Camera resolution height", camHeightInput);
    QObject::connect(camPredefinedCBox, &QCheckBox::toggled, camHeightInput,
                     &QWidget::setEnabled);
}

ServerPage::~ServerPage()
{
}
