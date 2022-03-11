#pragma once

class ILoadable
{
public:
    ILoadable()
    {
    }

    virtual void load() = 0;
};
