#pragma once

#include <QtCore>

#include "interfaces/ISavableLoadable.hpp"

/**
 * Setting manager class. Used to hook up settings to a few functions for easier
 * saving and loading later
 */
class SettingManager : public QObject, public ISavableLoadable
{
    Q_OBJECT

public:
    /**
     * Constructor
     * @param parent Parent object
     */
    SettingManager(QObject *parent);
    /**
     * Destructor
     */
    ~SettingManager();

    /**
     * Read a single setting from the system in a closs-platform way
     * @param key Identifier string for the setting. Paths for categorization
     * like /group1/group11/settingName can be used
     * @param defaultVal Default value to return if no setting is found
     * @return The value of the setting
     */
    template <typename T>
    T readSetting(const QString &key, const T &defaultVal) const
    {
        QVariant var = this->qsettings_.value(key, defaultVal);

        if (!var.canConvert<T>())
            return defaultVal;

        return var.value<T>();
    }

    /**
     * Write a single setting to the system in a closs-platform way
     * @param key Identifier string for the setting. Paths for categorization
     * like /group1/group11/settingName can be used
     * @param value Value to write
     */
    template <typename T>
    void writeSetting(const QString &key, const T &value)
    {
        this->qsettings_.setValue(key, value);
    }

    /**
     * Add setting to the list which is used to manage them
     * @param savableLoadable Setting to add
     */
    void addSavableLoadable(ISavableLoadable *savableLoadable);

    /**
     * Save all settings in the managed list
     */
    void save() override;
    /**
     * Load all settings in the managed list
     */
    void load() override;

private:
    QSettings qsettings_ = QSettings("SpaceMissionSimVis");
    std::vector<ISavableLoadable *> savableLoadables_;
};
