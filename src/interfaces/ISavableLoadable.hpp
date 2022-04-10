#pragma once

class ISavableLoadable
{
public:
    virtual ~ISavableLoadable()
    {
    }

    virtual void save() = 0;
    virtual void load() = 0;
};
