#pragma once

/**
 * Savable and loadable interface
 */
class ISavableLoadable
{
public:
    /**
     * Virtual destructor
     */
    virtual ~ISavableLoadable()
    {
    }

    /**
     * Save data
     */
    virtual void save() = 0;
    /**
     * Load data
     */
    virtual void load() = 0;
};
