#pragma once

#include <QtCore>
#include <QtWidgets>

#include "common/Resources.hpp"

/**
 * Connection display class
 */
class ConnectionDisplay : public QWidget
{
    Q_OBJECT

public:
    /**
     * Constructor
     * @param parent Parent widget
     * @param resources Resources object
     */
    ConnectionDisplay(QWidget *parent, Resources *const resources);
    /**
     * Destructor
     */
    ~ConnectionDisplay();

    enum class Status { Connected, Disconnected };
    /**
     * Update the displayed connection status
     * @param status The display status to set
     */
    void updateStatus(ConnectionDisplay::Status status);

private:
    Resources *const resources_;
    QLabel *connIcon_;
    QLabel *connLabel_;
};
