#include "SettingManager.hpp"

SettingManager::SettingManager(QObject *parent)
    : QObject(parent)
{
}

SettingManager::~SettingManager()
{
}

void SettingManager::addSavableLoadable(ISavableLoadable *savableLoadable)
{
    this->savableLoadables_.push_back(savableLoadable);
}

void SettingManager::save()
{
    for (auto &savable : this->savableLoadables_)
    {
        savable->save();
    }
}

void SettingManager::load()
{
    for (auto &savable : this->savableLoadables_)
    {
        savable->load();
    }
}
