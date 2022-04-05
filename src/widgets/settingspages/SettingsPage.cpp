#include "SettingsPage.hpp"

SettingsPage::SettingsPage(QWidget *parent, Settings *const settings)
    : QFrame(parent)
    , settings_(settings)
{
    //TODO: make the page scrollable
    //not an issue atm because there aren't that many settings
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
    QVBoxLayout *pageLayout = new QVBoxLayout(this);
    this->layout()->setAlignment(Qt::AlignTop);
}

SettingsPage::~SettingsPage()
{
}

QCheckBox *SettingsPage::createCheckBox(BoolSetting &setting,
                                        const QString &text)
{
    QCheckBox *checkbox = new QCheckBox(text, this);
    checkbox->setChecked(setting.value());

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
                     [&setting, lineEdit](const QString &newText) {
                         if (lineEdit->hasAcceptableInput())
                             setting.setValue(newText.toInt());
                     });

    return lineEdit;
}

QLineEdit *SettingsPage::createLineEdit(FloatSetting &setting, const float &min,
                                        const float &max, const int &decimals)
{
    QLineEdit *lineEdit = new QLineEdit(QString::number(setting.value()), this);
    lineEdit->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    lineEdit->setValidator(new QDoubleValidator(min, max, decimals, lineEdit));

    QObject::connect(lineEdit, &QLineEdit::textChanged, this,
                     [&setting, lineEdit](const QString &newText) {
                         if (lineEdit->hasAcceptableInput())
                             setting.setValue(newText.toDouble());
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
                     [&setting, lineEdit](const QString &newText) {
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
    QWidget *wrapper = new QWidget(this);

    QLineEdit *lineEdit = new QLineEdit(setting.value(), wrapper);
    lineEdit->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    QObject::connect(lineEdit, &QLineEdit::textChanged, this,
                     [&setting](const QString &newText) {
                         setting.setValue(newText);
                     });

    QPushButton *browseButton = new QPushButton("Browse", wrapper);
    QObject::connect(browseButton, &QPushButton::clicked, this, [=] {
        QString filePath =
            QFileDialog::getOpenFileName(this, caption, dir, filter);

        if (filePath.length() == 0)
        {
            return;
        }

        lineEdit->setText(filePath);
    });

    wrapper->setLayout(new HBoxLayout);
    wrapper->layout()->addWidget(lineEdit);
    wrapper->layout()->addWidget(browseButton);

    return wrapper;
}

QPushButton *SettingsPage::createColorPicker(QStringSetting &setting,
                                             const QString &btnText)
{
    QPushButton *colorButton = new QPushButton(btnText, this);

    colorButton->setAutoFillBackground(true);
    colorButton->setFlat(true);
    QPalette pal = colorButton->palette();
    pal.setColor(QPalette::Button, setting.value());
    colorButton->setPalette(pal);

    QColorDialog *colorDialog = new QColorDialog(setting.value(), this);
    QObject::connect(colorDialog, &QColorDialog::colorSelected, this,
                     [&setting, colorButton](const QColor &color) {
                         setting.setValue(color.name());

                         QPalette pal = colorButton->palette();
                         pal.setColor(QPalette::Button, color);
                         colorButton->setPalette(pal);
                     });

    QObject::connect(colorButton, &QPushButton::clicked, colorDialog,
                     &QColorDialog::show);

    return colorButton;
}
