#pragma once

#include <QtCore>

/**
 * Editor interface
 */
class IEditor
{
public:
    /**
     * Virtual destructor
     */
    virtual ~IEditor()
    {
    }

    /**
     * @return Whether the initially loaded data in the editor is modified or
     * not
     */
    virtual bool isModified() const = 0;
    /**
     * @return Text in the currently active line
     */
    virtual QString activeLineText() const = 0;
    /**
     * Jump to a line in the editor
     * @param lineNum The line to jump to
     */
    virtual void goToLine(int lineNum) = 0;

    /**
     * Save the contents of the editor in the current file
     */
    virtual int save() = 0;
    /**
     * Choose how to save editor contents
     */
    virtual int saveAs() = 0;
    /**
     * Load new contents into the editor
     */
    virtual int load() = 0;
    /**
     * Clear the editor's contents
     */
    virtual void clear() = 0;
};
