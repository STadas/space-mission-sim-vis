#pragma once

#include <QtCore>
#include <QtWidgets>

/**
 * Action class. QAction with custom functionality for convenience
 */
class Action : public QAction
{
    Q_OBJECT

public:
    /**
     * Copy all constructors from QAction
     */
    using QAction::QAction;

    /**
     * Set statusTip and toolTip at the same time
     * @param statusTip The status tip to set
     */
    void setStatusTip(const QString &statusTip);
};
