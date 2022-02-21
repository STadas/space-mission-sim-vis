#include "SimPreview.hpp"

SimPreview::SimPreview(QWidget *parent)
    : QWidget(parent)
    , imgLabel_(new QLabel(this))
{
    QLayout *layout = new VBoxLayout(this);
    this->layout()->addWidget(imgLabel_);
    this->imgLabel_->setSizePolicy(QSizePolicy::Expanding,
                                   QSizePolicy::Expanding);
    this->imgLabel_->setMinimumSize(1, 1);
    this->imgLabel_->installEventFilter(this);
}

SimPreview::~SimPreview()
{
}

void SimPreview::showPreview(unsigned char *&data, const unsigned long &size)
{
    /* this->imgLabel_->setText("showImage"); */
    QSize oldSize = this->size();

    this->pm_.loadFromData(QByteArray((char *)data, size), "PNG");
    this->imgLabel_->setPixmap(this->pm_);

    QSize newSize = this->size();

    newSize.setWidth(oldSize.width());
    QEvent resizeEvent = QResizeEvent(newSize, oldSize);
    this->eventFilter(this->imgLabel_, &resizeEvent);
}

bool SimPreview::eventFilter(QObject *object, QEvent *event)
{
    /* https://stackoverflow.com/a/68486532 */
    if (object == this->imgLabel_ && event->type() == QEvent::Resize)
    {
        if (!this->pm_.isNull())
        {
            this->imgLabel_->setPixmap(
                this->pm_.scaled(this->imgLabel_->size(), Qt::KeepAspectRatio));
            return true;
        }
    }
    return false;
}
