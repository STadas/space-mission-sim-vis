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
using QStringListSetting = GenericSetting<QStringList>;
using QByteArraySetting = GenericSetting<QByteArray>;

/**
 * Settings class. Stores all of the settings
 */
class Settings : public QObject, public ISavableLoadable
{
    Q_OBJECT

private:
    SettingManager *const manager_ = new SettingManager(this);

public:
    /**
     * Constructor
     * @param parent Parent object
     */
    Settings(QObject *parent);
    /**
     * Destructor
     */
    ~Settings();

    /**
     * Saves all of the settings
     */
    void save() override;
    /**
     * Loads all of the settings
     */
    void load() override;

    // server executable
    QStringSetting serverPath = {"server/exec/path", "", this->manager_};
    QStringSetting serverIni = {"server/exec/ini", "", this->manager_};
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

    // commands
    IntSetting commandsStepMsDelay = {"commands/step/msDelay", 1000,
                                      this->manager_};
    BoolSetting commandsStepStopUnimplemented = {
        "commands/step/StopUnimplemented", false, this->manager_};

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

    // editor recent files
    QStringListSetting recentFiles = {"editor/recentFiles", {}, this->manager_};
    IntSetting maxRecentFiles = {"editor/maxRecentFiles", 5, this->manager_};

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

    // coords vis planes
    FloatSetting coordsVisSurfacePlaneScale = {"coordsVis/surface/scale",
                                               100.0f, this->manager_};
    FloatSetting coordsVisCamFarPlane = {"coordsVis/cam/farPlane", 10000.0f,
                                         this->manager_};
};
