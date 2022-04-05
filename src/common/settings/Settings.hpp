#pragma once

#include <QtCore>
#include <QtWidgets>

#include "common/settings/GenericSetting.hpp"
#include "common/settings/SettingManager.hpp"
#include "interfaces/ISavableLoadable.hpp"

using BoolSetting = GenericSetting<bool>;
using IntSetting = GenericSetting<int>;
using FloatSetting = GenericSetting<float>;
using DoubleSetting = GenericSetting<double>;
using StringSetting = GenericSetting<std::string>;
using QStringSetting = GenericSetting<QString>;
using QByteArraySetting = GenericSetting<QByteArray>;

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

    // server executable
    QStringSetting serverPath = {"server/exec/path", "", this->manager_};
    QStringSetting serverFlags = {"server/exec/flags", "", this->manager_};

    // server connection
    QStringSetting serverAddress = {"server/address", "localhost",
                                    this->manager_};
    IntSetting serverPort = {"server/port", 10363, this->manager_};

    // server cam
    BoolSetting serverCamPredefined = {"server/cam/usePredefined", true,
                                       this->manager_};
    IntSetting serverCamWidth = {"server/cam/width", 1024, this->manager_};
    IntSetting serverCamHeight = {"server/cam/height", 1024, this->manager_};

    // windows' geometry/state
    QByteArraySetting stateMainWindow = {
        "rect/mainWindow/state", {}, this->manager_};
    QByteArraySetting geometryMainWindow = {
        "rect/mainWindow/geometry", {}, this->manager_};
    QByteArraySetting geometrySettingsDialog = {
        "rect/settingsDialog/geometry", {}, this->manager_};

    // editor colors
    QStringSetting currLineHighlightColor = {"editor/currLine/hightlightColor",
                                             "#ffff00", this->manager_};

    // coords vis colors
    QStringSetting coordsVisBackgroundColor = {"coordsVis/backgroundColor",
                                               "#000000", this->manager_};
    QStringSetting coordsVisPlaneColor = {"coordsVis/plane/color", "#333333",
                                          this->manager_};
    QStringSetting coordsVisDefaultColor = {"coordsVis/points/defaultColor",
                                            "#ffffff", this->manager_};
    QStringSetting coordsVisActiveColor = {"coordsVis/points/activeColor",
                                           "#ffff00", this->manager_};

    // coords vis points sizes
    FloatSetting coordsVisDefaultPSize = {"coordsVis/points/defaultSize", 4.0f,
                                          this->manager_};
    FloatSetting coordsVisActivePSize = {"coordsVis/points/activeSize", 8.0f,
                                         this->manager_};
};
