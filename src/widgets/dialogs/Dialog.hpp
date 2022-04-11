#pragma once

#include <QtCore>
#include <QtWidgets>

#include "interfaces/ISavableLoadable.hpp"

/**
 * Dialog class. QDialog with some extra functionality
 */
class Dialog : public QDialog, public ISavableLoadable
{
    Q_OBJECT

public:
    /**
     * Constructor
     * @param parent Parent widget
     */
    Dialog(QWidget *parent);
    /**
     * Destructor
     */
    ~Dialog();

    /**
     * Override the close event to do some stuff before the destructor, like
     * saving stuff
     * @param event Event passed by the GUI framework
     */
    void closeEvent(QCloseEvent *event) override;

protected:
    /**
     * Save stuff like the window's geometry and state
     */
    void save() override = 0;
    /**
     *  Load stuff like the windows's geometry and state
     */
    void load() override = 0;
};
