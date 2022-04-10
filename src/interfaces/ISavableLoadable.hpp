#pragma once

class ISavableLoadable
{
public:
    ISavableLoadable()
    {
    }

    virtual void save() = 0;
    virtual void load() = 0;
};
