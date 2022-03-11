#pragma once

#include <QtCore>

#include "interfaces/ISavableLoadable.hpp"

class SettingManager : public QObject, ISavableLoadable
{
    Q_OBJECT

public:
    SettingManager(QObject *parent);
    ~SettingManager();

    template <typename T>
    T readSetting(const QString &key, const T &defaultVal) const
    {
        QVariant var = this->qsettings_.value(key, defaultVal);

        if (!var.canConvert<T>())
            return defaultVal;

        return var.value<T>();
    }

    template <typename T>
    void writeSetting(const QString &key, const T &value)
    {
        this->qsettings_.setValue(key, value);
    }

    void addSavableLoadable(ISavableLoadable *savableLoadable);
    void save() override;
    void load() override;

private:
    QSettings qsettings_ = QSettings("SpaceMissionSimVis");
    std::vector<ISavableLoadable *> savableLoadables_;
};
