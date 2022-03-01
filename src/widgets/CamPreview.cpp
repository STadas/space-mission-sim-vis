#include "CamPreview.hpp"

CamPreview::CamPreview(QWidget *parent)
    : QWidget(parent)
    , imgLabel_(new QLabel(this))
{
    QLayout *layout = new VBoxLayout(this);
    this->imgLabel_->setSizePolicy(QSizePolicy::Expanding,
                                   QSizePolicy::Expanding);
    this->imgLabel_->setMinimumSize(1, 1);
    this->imgLabel_->installEventFilter(this);

    this->layout()->addWidget(this->imgLabel_);
}

CamPreview::~CamPreview()
{
}

void CamPreview::showPreview(unsigned char *data, const unsigned long &size)
{
    QSize oldSize = this->size();

    this->pm_.loadFromData(QByteArray((char *)data, size), "PNG");
    this->imgLabel_->setPixmap(this->pm_);
    delete data;

    QSize newSize = this->size();

    newSize.setWidth(oldSize.width());
    QEvent resizeEvent = QResizeEvent(newSize, oldSize);
    this->eventFilter(this->imgLabel_, &resizeEvent);
}

bool CamPreview::eventFilter(QObject *object, QEvent *event)
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
