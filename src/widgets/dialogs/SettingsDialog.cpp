#include "SettingsDialog.hpp"
#include <qlistwidget.h>

SettingsDialog::SettingsDialog(QWidget *parent, Settings *const settings)
    : QDialog(parent)
    , pageSelect_(new QListWidget(this))
    , pages_(new QStackedWidget(this))
    , settings_(settings)
{
    this->setWindowTitle("Settings");

    this->setLayout(new QVBoxLayout);

    QWidget *settingsWrapper = new QWidget(this);
    settingsWrapper->setLayout(new QHBoxLayout);
    settingsWrapper->layout()->setMargin(0);
    this->layout()->addWidget(settingsWrapper);

    QWidget *buttonsWrapper = new QWidget(this);
    buttonsWrapper->setLayout(new QHBoxLayout);
    buttonsWrapper->layout()->setMargin(0);
    buttonsWrapper->layout()->setAlignment(Qt::AlignRight);
    this->layout()->addWidget(buttonsWrapper);

    settingsWrapper->layout()->addWidget(this->pageSelect_);
    settingsWrapper->layout()->addWidget(this->pages_);

    this->okButton = new QPushButton("Ok", this);
    buttonsWrapper->layout()->addWidget(this->okButton);
    connect(this->okButton, &QPushButton::clicked, this,
            &SettingsDialog::onOkClicked);

    this->cancelButton = new QPushButton("Cancel", this);
    buttonsWrapper->layout()->addWidget(this->cancelButton);
    connect(this->cancelButton, &QPushButton::clicked, this,
            &SettingsDialog::onCancelClicked);

    this->createPages();
    this->pageSelect_->setFixedWidth(
        this->pageSelect_->sizeHintForColumn(0) +
        this->pageSelect_->verticalScrollBar()->sizeHint().width());
    if (this->pageSelect_->count() > 0)
    {
        this->pageSelect_->setCurrentRow(0);
    }
}

void SettingsDialog::createPages()
{
    connect(this->pageSelect_, &QListWidget::currentRowChanged, this,
            &SettingsDialog::onCurrentChanged);

    QWidget *serverPage = new ServerPage(this, this->settings_);
    QAction *actServerPage = new QAction("Server", this);
    this->addPage(actServerPage, serverPage);
}

void SettingsDialog::addPage(QAction *action, QWidget *page)
{
    this->pages_->addWidget(page);
    QListWidgetItem *item = new QListWidgetItem(action->text());
    item->setTextAlignment(Qt::AlignCenter);
    this->pageSelect_->addItem(item);
}

void SettingsDialog::onCurrentChanged(const int &current)
{
    this->pages_->setCurrentIndex(current);
}

void SettingsDialog::onOkClicked()
{
    this->settings_->save();
    this->close();
}

void SettingsDialog::onCancelClicked()
{
    this->settings_->load();
    this->close();
}
