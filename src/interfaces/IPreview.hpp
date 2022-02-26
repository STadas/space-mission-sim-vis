#pragma once

#include <stddef.h>

class IPreview
{
public:
    ~IPreview(){};

    virtual void changePreview(unsigned char *data,
                               const unsigned long &size) = 0;
};
