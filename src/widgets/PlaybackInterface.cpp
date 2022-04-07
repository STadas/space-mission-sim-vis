#include "PlaybackInterface.hpp"

PlaybackInterface::PlaybackInterface(QWidget *parent, Settings *const settings)
    : QWidget(parent)
    , progressBar_(new ProgressBar(this))
    , settings_(settings)
    , buttonWrapper_(new QWidget(this))
{
    this->setLayout(new VBoxLayout);
    this->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);

    this->buttonWrapper_->setLayout(new HBoxLayout);
    this->buttonWrapper_->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    this->layout()->addWidget(this->progressBar_);
    this->layout()->addWidget(this->buttonWrapper_);
    this->layout()->setAlignment(this->buttonWrapper_, Qt::AlignCenter);

    QSpinBox *msDelayBox = new QSpinBox(this);
    msDelayBox->setMinimum(0);
    msDelayBox->setMaximum(INT_MAX);
    msDelayBox->setSingleStep(100);
    msDelayBox->setValue(this->settings_->commandsStepMsDelay.value());
    QObject::connect(msDelayBox, QOverload<int>::of(&QSpinBox::valueChanged),
                     this, [this](const int &newVal) {
                         this->settings_->commandsStepMsDelay.setValue(newVal);
                         this->settings_->commandsStepMsDelay.save();
                     });
    this->buttonWrapper_->layout()->addWidget(new QLabel("Step delay (ms) "));
    this->buttonWrapper_->layout()->addWidget(msDelayBox);
    this->buttonWrapper_->layout()->addItem(new QSpacerItem(10, 0));
}

PlaybackInterface::~PlaybackInterface()
{
}

void PlaybackInterface::addButton(QAction *action)
{
    QToolButton *newButton = new QToolButton(this);
    newButton->setDefaultAction(action);

    this->buttonWrapper_->layout()->addWidget(newButton);
}
