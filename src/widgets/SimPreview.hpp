#pragma once

#include <QtCore>
#include <QtWidgets>

#include "common/VBoxLayout.hpp"
#include "interfaces/IPreview.hpp"

class SimPreview : public QWidget, IPreview
{
    Q_OBJECT

public:
    SimPreview(QWidget *parent = nullptr);
    ~SimPreview() override;

    void showPreview(unsigned char *&data, const unsigned long &size) override;
    bool eventFilter(QObject *object, QEvent *event) override;

private:
    QLabel *imgLabel_;
    QPixmap pm_;
};
