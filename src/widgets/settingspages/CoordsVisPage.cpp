#include "CoordsVisPage.hpp"

CoordsVisPage::CoordsVisPage(QWidget *parent, Settings *settings)
    : SettingsPage(parent, settings)
{
    QGroupBox *colorsGroup = new QGroupBox("Colors", this);
    QFormLayout *colorsLayout = new QFormLayout(colorsGroup);
    this->layout()->addWidget(colorsGroup);

    colorsLayout->addRow(
        "Background color",
        this->createColorPicker(this->settings_->coordsVisBackgroundColor));

    colorsLayout->addRow(
        "Plane color",
        this->createColorPicker(this->settings_->coordsVisPlaneColor));

    colorsLayout->addRow(
        "Default point/line color",
        this->createColorPicker(this->settings_->coordsVisDefaultColor));

    colorsLayout->addRow(
        "Active point/line color",
        this->createColorPicker(this->settings_->coordsVisActiveColor));

    QGroupBox *sizesGroup = new QGroupBox("Point sizes", this);
    QFormLayout *sizesLayout = new QFormLayout(sizesGroup);
    this->layout()->addWidget(sizesGroup);

    sizesLayout->addRow(
        "Default point size",
        this->createLineEdit(this->settings_->coordsVisDefaultPSize, 0));

    sizesLayout->addRow(
        "Active point size",
        this->createLineEdit(this->settings_->coordsVisActivePSize, 0));
}

CoordsVisPage::~CoordsVisPage()
{
}
