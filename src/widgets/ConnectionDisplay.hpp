#pragma once

#include <QtCore>
#include <QtWidgets>

#include "common/Resources.hpp"

class ConnectionDisplay : public QWidget
{
    Q_OBJECT

public:
    ConnectionDisplay(QWidget *parent, Resources *const resources);
    ~ConnectionDisplay();

    enum class Status { Connected, Disconnected };
    void updateStatus(ConnectionDisplay::Status status);

private:
    Resources *const resources_;
    QLabel *connIcon_;
    QLabel *connLabel_;
};
