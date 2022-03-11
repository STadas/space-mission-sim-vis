#pragma once

#include <QtCore>

class IEditor
{
public:
    virtual ~IEditor()
    {
    }

    virtual bool isModified() = 0;
    virtual QString activeLineText() = 0;
    virtual void goToLine(int lineNum) = 0;

    virtual int save() = 0;
    virtual int saveAs() = 0;
    virtual int load() = 0;
    virtual void clear() = 0;
};
