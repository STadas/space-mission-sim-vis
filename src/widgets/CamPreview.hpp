#pragma once

#include <QtCore>
#include <QtWidgets>

#include "common/VBoxLayout.hpp"

/**
 * Camera preview class
 */
class CamPreview : public QWidget
{
    Q_OBJECT

public:
    /**
     * Constructor
     * @param parent Parent widget
     */
    CamPreview(QWidget *parent = nullptr);
    /**
     * Destructor
     */
    ~CamPreview() override;

    /**
     * Show image on the widget
     * @param data Image data to show
     */
    void showPreview(QByteArray data);

    /**
     * Override eventFilter for custom scaling behavior
     * @param object Object passed by the GUI framework
     * @event Event passed by the gUI framework
     * @return Whether the event was intercepted or not
     */
    bool eventFilter(QObject *object, QEvent *event) override;

private:
    QLabel *imgLabel_;
    QPixmap pm_;
};
