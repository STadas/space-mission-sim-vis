#include "ConnectionDisplay.hpp"

ConnectionDisplay::ConnectionDisplay(QWidget *parent,
                                     Resources *const resources)
    : QWidget(parent)
    , resources_(resources)
    , connIcon_(new QLabel(this))
    , connLabel_(new QLabel("Disconnected", this))
{
    this->setLayout(new QHBoxLayout);
    this->layout()->setMargin(0);
    this->layout()->addWidget(this->connIcon_);
    this->layout()->addWidget(this->connLabel_);

    this->connIcon_->setPixmap(this->resources_->iconWarn.pixmap(16));
}

ConnectionDisplay::~ConnectionDisplay()
{
}

void ConnectionDisplay::updateStatus(ConnectionDisplay::Status status)
{
    switch (status)
    {
        case ConnectionDisplay::Status::Connected: {
            this->connIcon_->setPixmap(this->resources_->iconOk.pixmap(16));
            this->connLabel_->setText("Connected");
            break;
        }
        case ConnectionDisplay::Status::Disconnected: {
            this->connIcon_->setPixmap(this->resources_->iconWarn.pixmap(16));
            this->connLabel_->setText("Disconnected");
            break;
        }
    }
}
