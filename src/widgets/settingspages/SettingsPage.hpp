#pragma once

#include <QtCore>
#include <QtWidgets>

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

    QCheckBox *createCheckBox(BoolSetting &setting, const QString &text);

    QLineEdit *createLineEdit(QStringSetting &setting);
    QLineEdit *createLineEdit(IntSetting &setting, const int &min,
                              const int &max);
    QLineEdit *createLineEdit(DoubleSetting &setting, const double &min,
                              const double &max, const int &decimals);
    QWidget *createLineEditBrowse(QStringSetting &setting,
                                  const QString &caption, const QString &dir,
                                  const QString &filter);
};
