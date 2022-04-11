#include "SettingsDialog.hpp"

SettingsDialog::SettingsDialog(QWidget *parent, Settings *const settings,
                               Resources *const resources)
    : Dialog(parent)
    , pageSelect_(new QListWidget(this))
    , pages_(new QStackedWidget(this))
    , settings_(settings)
    , resources_(resources)
{
    this->setAttribute(Qt::WA_DeleteOnClose, true);
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
    QObject::connect(this->okButton, &QPushButton::clicked, this,
                     &SettingsDialog::onOkClicked);

    this->cancelButton = new QPushButton("Cancel", this);
    buttonsWrapper->layout()->addWidget(this->cancelButton);
    QObject::connect(this->cancelButton, &QPushButton::clicked, this,
                     &SettingsDialog::onCancelClicked);

    this->createPages();
    this->pageSelect_->setFixedWidth(
        this->pageSelect_->sizeHintForColumn(0) +
        this->pageSelect_->verticalScrollBar()->sizeHint().width());
    if (this->pageSelect_->count() > 0)
    {
        this->pageSelect_->setCurrentRow(0);
    }

    this->load();
}

SettingsDialog::~SettingsDialog()
{
}

void SettingsDialog::save()
{
    this->settings_->geometrySettingsDialog.setValue(this->saveGeometry());
}

void SettingsDialog::load()
{
    if (this->settings_->geometrySettingsDialog.value() != QByteArray())
        this->restoreGeometry(this->settings_->geometrySettingsDialog.value());
}

void SettingsDialog::createPages()
{
    QObject::connect(this->pageSelect_, &QListWidget::currentRowChanged, this,
                     &SettingsDialog::onCurrentChanged);

    this->addPage(new QAction(this->resources_->iconDisplay, "Server", this),
                  new ServerPage(this, this->settings_));

    this->addPage(
        new QAction(this->resources_->iconCursorExecute, "Commands", this),
        new CommandsPage(this, this->settings_));

    this->addPage(new QAction(this->resources_->iconEditor, "Editor", this),
                  new EditorPage(this, this->settings_));

    this->addPage(
        new QAction(this->resources_->iconCoordinate, "Coord-Vis", this),
        new CoordsVisPage(this, this->settings_));
}

void SettingsDialog::addPage(QAction *action, SettingsPage *page)
{
    this->pages_->addWidget(page);
    QListWidgetItem *item = new QListWidgetItem(action->icon(), action->text());
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
