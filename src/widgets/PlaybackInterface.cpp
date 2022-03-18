#include "PlaybackInterface.hpp"

PlaybackInterface::PlaybackInterface(QWidget *parent)
    : QWidget(parent)
    , progressBar_(new ProgressBar(this))
    , buttonWrapper_(new QWidget(this))
{
    this->setLayout(new VBoxLayout);
    this->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);

    this->buttonWrapper_->setLayout(new HBoxLayout);
    this->buttonWrapper_->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    this->layout()->addWidget(this->progressBar_);
    this->layout()->addWidget(this->buttonWrapper_);
    this->layout()->setAlignment(this->buttonWrapper_, Qt::AlignCenter);
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
