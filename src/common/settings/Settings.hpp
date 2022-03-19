#pragma once

#include <QtCore>

#include "common/settings/GenericSetting.hpp"
#include "common/settings/SettingManager.hpp"
#include "interfaces/ISavableLoadable.hpp"

using BoolSetting = GenericSetting<bool>;
using IntSetting = GenericSetting<int>;
using FloatSetting = GenericSetting<float>;
using DoubleSetting = GenericSetting<double>;
using StringSetting = GenericSetting<std::string>;
using QStringSetting = GenericSetting<QString>;

class Settings : public QObject, public ISavableLoadable
{
    Q_OBJECT

private:
    SettingManager *const manager_ = new SettingManager(this);

public:
    Settings(QObject *parent);
    ~Settings();

    void save() override;
    void load() override;

    QStringSetting serverPath = {"server/exec/path", "", this->manager_};
    QStringSetting serverFlags = {"server/exec/flags", "", this->manager_};
    QStringSetting serverAddress = {"server/address", "localhost",
                                    this->manager_};
    IntSetting serverPort = {"server/port", 10363, this->manager_};
};
