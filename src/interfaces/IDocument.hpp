#pragma once

class IDocument
{
public:
    virtual ~IDocument(){};

    virtual bool isModified() = 0;

    virtual int save() = 0;
    virtual int saveAs() = 0;
    virtual int load() = 0;
    virtual void clear() = 0;
};
