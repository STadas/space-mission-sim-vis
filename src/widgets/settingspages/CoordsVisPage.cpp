#include "CoordsVisPage.hpp"

CoordsVisPage::CoordsVisPage(QWidget *parent, Settings *settings)
    : SettingsPage(parent, settings)
{
    QFormLayout *form = new QFormLayout(this);
    form->setMargin(0);

    form->addRow(
        "Background color",
        this->createColorPicker(this->settings_->coordsVisBackgroundColor));

    form->addRow("Plane color",
                 this->createColorPicker(this->settings_->coordsVisPlaneColor));

    form->addRow(
        "Default point/line color",
        this->createColorPicker(this->settings_->coordsVisDefaultColor));

    form->addRow(
        "Active point/line color",
        this->createColorPicker(this->settings_->coordsVisActiveColor));

    form->addRow("Default point size",
                 this->createLineEdit(this->settings_->coordsVisDefaultPSize, 0,
                                      FLT_MAX, 5));

    form->addRow("Active point size",
                 this->createLineEdit(this->settings_->coordsVisActivePSize, 0,
                                      FLT_MAX, 5));
}

CoordsVisPage::~CoordsVisPage()
{
}
