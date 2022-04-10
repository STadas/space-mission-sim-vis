#pragma once

#include <QtCore>
#include <QtWidgets>

#include "common/HBoxLayout.hpp"
#include "common/VBoxLayout.hpp"
#include "common/settings/Settings.hpp"
#include "widgets/ProgressBar.hpp"

class PlaybackInterface : public QWidget
{
    Q_OBJECT

public:
    PlaybackInterface(QWidget *parent, Settings *const settings);
    ~PlaybackInterface();

    void addButton(QAction *action);
    ProgressBar *progressBar();

private:
    Settings *const settings_;

    QWidget *buttonWrapper_;
    ProgressBar *progressBar_;
};
