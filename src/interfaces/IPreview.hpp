#pragma once

class IPreview
{
public:
    ~IPreview()
    {
    }

    virtual void showPreview(unsigned char *data,
                             const unsigned long &size) = 0;
};
