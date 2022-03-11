#include "SettingsPage.hpp"

SettingsPage::SettingsPage(QWidget *parent, Settings *const settings)
    : QFrame(parent)
    , settings_(settings)
{
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

SettingsPage::~SettingsPage()
{
}

QCheckBox *SettingsPage::createCheckBox(BoolSetting &setting,
                                        const QString &text)
{
    QCheckBox *checkbox = new QCheckBox(text, this);

    QObject::connect(checkbox, &QCheckBox::toggled, this,
                     [&setting](const bool &newVal) {
                         setting.setValue(newVal);
                     });

    return checkbox;
}

QLineEdit *SettingsPage::createLineEdit(QStringSetting &setting)
{
    QLineEdit *lineEdit = new QLineEdit(setting.value(), this);
    lineEdit->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);

    QObject::connect(lineEdit, &QLineEdit::textChanged, this,
                     [&setting](const QString &newText) {
                         setting.setValue(newText);
                     });

    return lineEdit;
}

QLineEdit *SettingsPage::createLineEdit(IntSetting &setting, const int &min,
                                        const int &max)
{
    QLineEdit *lineEdit = new QLineEdit(QString::number(setting.value()), this);
    lineEdit->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    lineEdit->setValidator(new QIntValidator(min, max, lineEdit));

    QObject::connect(lineEdit, &QLineEdit::textChanged, this,
                     [&setting, &lineEdit](const QString &newText) {
                         if (lineEdit->hasAcceptableInput())
                             setting.setValue(newText.toInt());
                     });

    return lineEdit;
}

QLineEdit *SettingsPage::createLineEdit(DoubleSetting &setting,
                                        const double &min, const double &max,
                                        const int &decimals)
{
    QLineEdit *lineEdit = new QLineEdit(QString::number(setting.value()), this);
    lineEdit->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    lineEdit->setValidator(new QDoubleValidator(min, max, decimals, lineEdit));

    QObject::connect(lineEdit, &QLineEdit::textChanged, this,
                     [&setting, &lineEdit](const QString &newText) {
                         if (lineEdit->hasAcceptableInput())
                             setting.setValue(newText.toDouble());
                     });

    return lineEdit;
}

QWidget *SettingsPage::createLineEditBrowse(QStringSetting &setting,
                                            const QString &caption,
                                            const QString &dir,
                                            const QString &filter)
{
    QWidget *browseWrapper = new QWidget(this);

    QLineEdit *lineEdit = new QLineEdit(setting.value(), browseWrapper);
    lineEdit->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    QObject::connect(lineEdit, &QLineEdit::textChanged, this,
                     [&setting](const QString &newText) {
                         setting.setValue(newText);
                     });

    QPushButton *browseButton = new QPushButton("Browse", browseWrapper);
    QObject::connect(browseButton, &QPushButton::clicked, this, [=] {
        QString filePath =
            QFileDialog::getOpenFileName(this, caption, dir, filter);

        if (filePath.length() == 0)
        {
            return;
        }

        lineEdit->setText(filePath);
    });

    browseWrapper->setLayout(new HBoxLayout);
    browseWrapper->layout()->addWidget(lineEdit);
    browseWrapper->layout()->addWidget(browseButton);

    return browseWrapper;
}
