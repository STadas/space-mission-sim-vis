#include "Settings.hpp"

Settings::Settings(QObject *parent)
    : QObject(parent)
{
}

Settings::~Settings()
{
}

void Settings::save()
{
    this->manager_->save();
}

void Settings::load()
{
    this->manager_->load();
}
