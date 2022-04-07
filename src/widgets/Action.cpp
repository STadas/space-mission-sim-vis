#include "Action.hpp"

void Action::setStatusTip(const QString &statusTip)
{
    QAction::setStatusTip(statusTip);
    QAction::setToolTip(statusTip);
}
