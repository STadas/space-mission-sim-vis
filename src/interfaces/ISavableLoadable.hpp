#pragma once

#include "interfaces/ILoadable.hpp"
#include "interfaces/ISavable.hpp"

class ISavableLoadable : public ISavable, ILoadable
{
public:
    ISavableLoadable()
    {
    }

    virtual void save() override = 0;
    virtual void load() override = 0;
};
