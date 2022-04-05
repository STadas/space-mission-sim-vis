#pragma once

#include <QtCore>
#include <QtWidgets>

#include <float.h>

#include "common/HBoxLayout.hpp"
#include "common/settings/Settings.hpp"

class SettingsPage : public QFrame
{
    Q_OBJECT

public:
    SettingsPage(QWidget *parent, Settings *const settings);
    ~SettingsPage();

protected:
    Settings *const settings_;

    QCheckBox *createCheckBox(BoolSetting &setting, const QString &text = "");

    QLineEdit *createLineEdit(QStringSetting &setting);
    QLineEdit *createLineEdit(IntSetting &setting, const int &min = INT_MIN,
                              const int &max = INT_MAX);
    QLineEdit *createLineEdit(DoubleSetting &setting, const double &min = DBL_MIN,
                              const double &max = DBL_MAX, const int &decimals = DBL_DIG);
    QLineEdit *createLineEdit(FloatSetting &setting, const float &min = FLT_MIN,
                              const float &max = FLT_MAX, const int &decimals = FLT_DIG);
    QWidget *createLineEditBrowse(QStringSetting &setting,
                                  const QString &caption, const QString &dir,
                                  const QString &filter);

    QPushButton *createColorPicker(QStringSetting &setting,
                                   const QString &btnText = "");
};
