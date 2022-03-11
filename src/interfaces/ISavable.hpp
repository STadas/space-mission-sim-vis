#pragma once

class ISavable
{
public:
    ISavable()
    {
    }

    virtual void save() = 0;
};
