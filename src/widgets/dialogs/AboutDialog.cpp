#include "AboutDialog.hpp"

AboutDialog::AboutDialog(QWidget *parent, Resources *const resources)
    : QDialog(parent)
    , resources_(resources)
{
    this->setAttribute(Qt::WA_DeleteOnClose, true);
    this->setWindowTitle("About");
    /* this->setMaximumWidth(512); */

    this->setLayout(new QVBoxLayout);

    QGroupBox *descriptionGroup = new QGroupBox("About", this);
    descriptionGroup->setLayout(new QVBoxLayout);
    this->layout()->addWidget(descriptionGroup);

    QLabel *descriptionLabel = new QLabel(this);
    descriptionLabel->setText(
        "SpaceMissionSimVis is an application that is meant to make producing "
        "PANGU spacecraft mission simulations easier by making the process "
        "more interactive and intuitive.");
    descriptionLabel->setWordWrap(true);
    descriptionGroup->layout()->addWidget(descriptionLabel);

    QGroupBox *licensesGroup =
        new QGroupBox("Open source software used and their licenses", this);
    QFormLayout *formLayout = new QFormLayout(licensesGroup);
    this->layout()->addWidget(licensesGroup);

    // let's load big strings like licenses only when we need them
    this->addLicense(formLayout, "Qt Framework", ":/licenses/qt",
                     "https://www.qt.io/");
    this->addLicense(formLayout, "Breeze Icons", ":/licenses/breeze-icons",
                     "https://develop.kde.org/frameworks/breeze-icons/");
}

AboutDialog::~AboutDialog()
{
}

void AboutDialog::addLicense(QFormLayout *layout, const QString &name,
                             const QString &licensePath, const QString &website)
{
    QLabel *nameLabel = new QLabel(this);
    if (website.length() > 0)
    {
        nameLabel->setOpenExternalLinks(true);
        nameLabel->setText("<a href=\"" + website + "\">" + name + "</a>");
    }
    else
    {
        nameLabel->setText(name);
    }

    QLabel *licenseLabel =
        new QLabel("<a href=\"" + licensePath + "\">LICENSE</a>", this);
    QObject::connect(licenseLabel, &QLabel::linkActivated,
                     [this, name](const QString &resourcePath) {
                         QDialog *dialog = new QDialog(this);
                         dialog->setWindowTitle("License for " + name);
                         dialog->setAttribute(Qt::WA_DeleteOnClose, true);

                         dialog->setLayout(new QVBoxLayout);
                         QFile file(resourcePath);
                         file.open(QIODevice::ReadOnly);

                         QTextEdit *textEdit = new QTextEdit(dialog);
                         textEdit->setReadOnly(true);
                         textEdit->setText(file.readAll());
                         dialog->layout()->addWidget(textEdit);

                         dialog->show();
                     });

    layout->addRow(nameLabel, licenseLabel);
}
