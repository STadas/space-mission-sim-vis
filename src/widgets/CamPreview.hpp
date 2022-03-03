#pragma once

#include <QtCore>
#include <QtWidgets>

#include "common/VBoxLayout.hpp"
#include "interfaces/IPreview.hpp"
#include "widgets/ProgressBar.hpp"

class CamPreview : public QWidget, IPreview
{
    Q_OBJECT

public:
    CamPreview(QWidget *parent = nullptr);
    ~CamPreview() override;

    void showPreview(unsigned char *data, const unsigned long &size) override;

    bool eventFilter(QObject *object, QEvent *event) override;

private:
    QLabel *imgLabel_;
    QPixmap pm_;
};
