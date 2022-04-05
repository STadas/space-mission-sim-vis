#include "EditorPage.hpp"

EditorPage::EditorPage(QWidget *parent, Settings *const settings)
    : SettingsPage(parent, settings)
{
    QGroupBox *colorsGroup = new QGroupBox("Colors", this);
    QFormLayout *colorsLayout = new QFormLayout(colorsGroup);
    this->layout()->addWidget(colorsGroup);

    colorsLayout->addRow(
        "Current line highlight color",
        this->createColorPicker(this->settings_->currLineHighlightColor,
                                "Sample text"));
}

EditorPage::~EditorPage()
{
}
