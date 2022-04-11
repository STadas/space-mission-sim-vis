#pragma once

#include <QtCore>
#include <QtWidgets>

#include "common/HBoxLayout.hpp"
#include "common/VBoxLayout.hpp"
#include "common/settings/Settings.hpp"
#include "widgets/ProgressBar.hpp"

/**
 * Playback interface class
 */
class PlaybackInterface : public QWidget
{
    Q_OBJECT

public:
    /**
     * Constructor
     * @param parent Parent widget
     * @param settings Settings object
     */
    PlaybackInterface(QWidget *parent, Settings *const settings);
    /**
     * Destructor
     */
    ~PlaybackInterface();

    /**
     * Add an action button to the interface
     * action Action to add the button for
     */
    void addButton(QAction *action);
    /**
     * @return The progress bar of the playback interface
     */
    ProgressBar *progressBar();

private:
    Settings *const settings_;

    QWidget *buttonWrapper_;
    ProgressBar *progressBar_;
};
