#pragma once

#include <QtCore>

#include "common/settings/SettingManager.hpp"
#include "interfaces/ISavableLoadable.hpp"

/**
 * Generic setting class. Templated, so it can be used for different setting
 * types
 */
template <typename T>
class GenericSetting : public ISavableLoadable
{
public:
    /**
     * Constructor
     * @param key Identifier string for the setting. Paths for categorization
     * like `/group1/group11/settingName` can be used
     * @param value Default value of the setting
     * @param manager Manager to use to save/load the setting
     */
    GenericSetting(const QString &key, const T &value,
                   SettingManager *const manager)
        : key_(key)
        , value_(value)
        , manager_(manager)
    {
        this->manager_->addSavableLoadable(
            static_cast<ISavableLoadable *>(this));
    }

    /**
     * Destructor
     */
    ~GenericSetting()
    {
    }

    /**
     * @return The value of the setting
     */
    T value() const
    {
        return this->value_;
    }

    /**
     * Setting value setter
     * @param value The value to set
     */
    void setValue(const T &value)
    {
        this->value_ = value;
    }

    /**
     * Save the setting
     */
    void save() override
    {
        this->manager_->template writeSetting<T>(this->key_, this->value_);
    }

    /**
     * Load the setting
     */
    void load() override
    {
        this->value_ =
            this->manager_->template readSetting<T>(this->key_, this->value_);
    }

private:
    const QString key_;
    T value_;
    SettingManager *const manager_;
};
