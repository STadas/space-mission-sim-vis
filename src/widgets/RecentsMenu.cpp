#include "RecentsMenu.hpp"

RecentsMenu::RecentsMenu(QWidget *parent, Settings *const settings, MessageController *const messageController)
    : QMenu(parent)
    , settings_(settings)
    , messageController_(messageController)
{
    this->setTitle("Open recent flight file");
}

RecentsMenu::~RecentsMenu()
{
}

void RecentsMenu::updateActions(Editor *const editor)
{
    this->clear();
    for (QString &pathStr : this->settings_->recentFiles.value())
    {
        QAction *recentFile = new QAction(pathStr, this);
        QObject::connect(recentFile, &QAction::triggered, this,
                         [this, editor, pathStr] {
                             if (editor->load(pathStr))
                             {
                                 emit this->messageController_->error(
                                     FileErr::OpenFail, this->parentWidget());
                             }
                         });
        this->addAction(recentFile);
    }
}
