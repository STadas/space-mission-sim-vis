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

    QGroupBox *miscGroup = new QGroupBox("Miscellaneous", this);
    QFormLayout *miscLayout = new QFormLayout(miscGroup);
    this->layout()->addWidget(miscGroup);

    miscLayout->addRow("Maximum recent files",
                       this->createLineEdit(this->settings_->maxRecentFiles));
}

EditorPage::~EditorPage()
{
}
