#pragma once

#include <QtCore>
#include <QtWidgets>

#include "common/VBoxLayout.hpp"
#include "widgets/ProgressBar.hpp"

class CamPreview : public QWidget
{
    Q_OBJECT

public:
    CamPreview(QWidget *parent = nullptr);
    ~CamPreview() override;

    void showPreview(QByteArray data, const unsigned long &size);

    bool eventFilter(QObject *object, QEvent *event) override;

private:
    QLabel *imgLabel_;
    QPixmap pm_;
};
