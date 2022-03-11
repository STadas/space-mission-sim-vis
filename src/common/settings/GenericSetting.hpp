#pragma once

#include <QtCore>

#include "common/settings/SettingManager.hpp"
#include "interfaces/ISavableLoadable.hpp"

template <typename T>
class GenericSetting : public ISavableLoadable
{
public:
    GenericSetting(const QString &key, const T &value,
                   SettingManager *const manager)
        : key_(key)
        , value_(value)
        , manager_(manager)
    {
        this->manager_->addSavableLoadable(
            static_cast<ISavableLoadable *>(this));
    }

    ~GenericSetting()
    {
    }

    T value() const
    {
        return this->value_;
    }

    void setValue(const T &value)
    {
        this->value_ = value;
    }

    void save() override
    {
        this->manager_->template writeSetting<T>(this->key_, this->value_);
    }

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
