#pragma once

#include <QtCore>
#include <QtWidgets>

#include <float.h>

#include "common/HBoxLayout.hpp"
#include "common/settings/Settings.hpp"

/**
 * Settings page class
 */
class SettingsPage : public QFrame
{
    Q_OBJECT

public:
    /**
     * Constructor
     * @param parent Parent widget
     * @param settings Settings object
     */
    SettingsPage(QWidget *parent, Settings *const settings);
    /**
     * Destructor
     */
    ~SettingsPage();

protected:
    /**
     * Create a checkbox for a setting
     * @param setting The setting to connect to the field
     * @param text Text to add to the right of the checkbox
     */
    QCheckBox *createCheckBox(BoolSetting &setting, const QString &text = "");

    /**
     * Create a line edit for a string setting
     * @param setting The setting to connect to the field
     */
    QLineEdit *createLineEdit(QStringSetting &setting);
    /**
     * Create a line edit for an integer setting
     * @param setting The setting to connect to the field
     * @param min The minimum value allowed in the field
     * @param min The maximum value allowed in the field
     */
    QLineEdit *createLineEdit(IntSetting &setting, const int &min = INT_MIN,
                              const int &max = INT_MAX);
    /**
     * Create a line edit for a double setting
     * @param setting The setting to connect to the field
     * @param min The minimum value allowed in the field
     * @param min The maximum value allowed in the field
     * @param decimals the maximum precision allowed in the field
     */
    QLineEdit *createLineEdit(DoubleSetting &setting,
                              const double &min = DBL_MIN,
                              const double &max = DBL_MAX,
                              const int &decimals = DBL_DIG);
    /**
     * Create a line edit for a float setting
     * @param setting The setting to connect to the field
     * @param min The minimum value allowed in the field
     * @param min The maximum value allowed in the field
     * @param decimals the maximum precision allowed in the field
     */
    QLineEdit *createLineEdit(FloatSetting &setting, const float &min = FLT_MIN,
                              const float &max = FLT_MAX,
                              const int &decimals = FLT_DIG);
    /**
     * Create a line edit with a browse button for a string setting
     * @param setting The setting to connect to the field
     * @param caption The title to display on the file browser
     * @param dir The directory to start the file browser in
     * @param filter The file filters available in the file browser
     */
    QWidget *createLineEditBrowse(QStringSetting &setting,
                                  const QString &caption, const QString &dir,
                                  const QString &filter);

    /**
     * Create a color picker button for a string setting
     * @param setting The setting to connect to the field
     * @param btnText The text to display on the button
     */
    QPushButton *createColorPicker(QStringSetting &setting,
                                   const QString &btnText = "");

    Settings *const settings_;
};
