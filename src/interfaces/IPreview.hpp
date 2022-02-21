#pragma once

#include <stddef.h>

class IPreview
{
public:
    ~IPreview(){};

    virtual void showPreview(unsigned char *&data, const size_t &size) = 0;
};
