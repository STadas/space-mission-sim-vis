#pragma once

#include <QtCore>
#include <QtWidgets>

#include "common/HBoxLayout.hpp"
#include "common/VBoxLayout.hpp"
#include "widgets/ProgressBar.hpp"

class PlaybackInterface : public QWidget
{
    Q_OBJECT

public:
    PlaybackInterface(QWidget *parent);
    ~PlaybackInterface();

    void addButton(QAction *action);

    ProgressBar *progressBar_;

private:
    QWidget *buttonWrapper_;
};
