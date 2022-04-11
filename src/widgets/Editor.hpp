#pragma once

#include <QtWidgets>

#include "common/settings/Settings.hpp"
#include "interfaces/IEditor.hpp"
#include "util/FileUtil.hpp"

/**
 * Editor class
 */
class Editor : public QPlainTextEdit, public IEditor
{
    Q_OBJECT

public:
    /**
     * Constructor
     * @param parent Parent widget
     * @param settings Settings object
     */
    Editor(QWidget *parent, Settings *const settings);
    /**
     * Destructor
     */
    ~Editor() override;

    /**
     * @return Whether the initially loaded data in the editor is modified or
     * not
     */
    bool isModified() const override;
    /**
     * @return Text in the currently active line
     */
    QString activeLineText() const override;
    /**
     * Jump to a line in the editor
     * @param lineNum The line to jump to
     */
    void goToLine(int lineNum) override;

    /**
     * Save the contents of the editor in the current file
     */
    int save() override;
    /**
     * Choose how to save editor contents
     */
    int saveAs() override;
    /**
     * Load new contents into the editor
     */
    int load() override;
    /**
     * Load new contents into the editor from file
     * @param pathStr Path to the file to load contents from
     * @return Error status
     */
    QFileDevice::FileError load(const QString &pathStr);
    /**
     * Clear the editor's contents
     */
    void clear() override;

    /**
     * Override the key press event to intercept keys we don't want to have any
     * effect
     * @param keyEvent Event passed by the GUI framework
     */
    void keyPressEvent(QKeyEvent *keyEvent) override;
    /**
     * Show custom context menu to allow for additional actions
     * @param pt The location at which the context menu should appear
     */
    void showContextMenu(const QPoint &pt);

    /**
     * Set additional context menu actions
     * @param actions List of actions to add to the default context menu
     */
    void setExtraContextMenuActions(const QList<QAction *> &actions);

signals:
    /**
     * Emitted when the list of recent file paths is updated
     */
    void recentsUpdated(Editor *editor);

private:
    /**
     * Highlight the currently active line in the editor
     */
    void highlightCurrentLine();
    /**
     * Update the list of recent file paths
     */
    void updateRecents();

    QString defaultSavePath_{};
    Settings *const settings_;
    QList<QAction *> extraContextMenuActions_;
};
