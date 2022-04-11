#pragma once

#include <QtCore>
#include <QtWidgets>

#include "common/Resources.hpp"

/**
 * About dialog class. Displays the application description, licenses, etc.
 */
class AboutDialog : public QDialog
{
    Q_OBJECT

public:
    /**
     * Constructor
     * @param parent Parent widget
     */
    AboutDialog(QWidget *parent, Resources *const resources);
    /**
     * Destructor
     */
    ~AboutDialog();

private:
    /**
     * Add a row with a software name, website and license into the form layout
     * @param layout Form layout to add the row into
     * @param name Name of the software
     * @param licensePath Path to the license file
     * @param website Url to the software's website
     */
    void addLicense(QFormLayout *layout, const QString &name,
                    const QString &licensePath, const QString &website = "");

    Resources *const resources_;
};
