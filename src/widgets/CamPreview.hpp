#pragma once

#include <QtCore>
#include <QtWidgets>

#include "common/VBoxLayout.hpp"

class CamPreview : public QWidget
{
    Q_OBJECT

public:
    CamPreview(QWidget *parent = nullptr);
    ~CamPreview() override;

    void showPreview(QByteArray data);

    bool eventFilter(QObject *object, QEvent *event) override;

private:
    QLabel *imgLabel_;
    QPixmap pm_;
};
