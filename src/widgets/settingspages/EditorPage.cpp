#include "EditorPage.hpp"

EditorPage::EditorPage(QWidget *parent, Settings *const settings)
    : SettingsPage(parent, settings)
{
    QFormLayout *form = new QFormLayout;
    form->setMargin(0);
    this->setLayout(form);

    form->addRow("Current line highlight color",
                 this->createColorPicker(
                     this->settings_->currLineHighlightColor, "Sample text"));
}

EditorPage::~EditorPage()
{
}
