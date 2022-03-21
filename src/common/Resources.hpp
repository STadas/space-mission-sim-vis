#pragma once

#include <QtCore>
#include <QtWidgets>

class Resources : public QObject
{
    Q_OBJECT

public:
    Resources(QObject *parent);
    ~Resources();

    QIcon iconDocumentNew = QIcon(":/icons/document-new");
    QIcon iconDocumentOpen = QIcon(":/icons/document-open");
    QIcon iconDocumentSave = QIcon(":/icons/document-save");
    QIcon iconDocumentSaveAs = QIcon(":/icons/document-save-as");

    QIcon iconEditUndo = QIcon(":/icons/edit-undo");
    QIcon iconEditRedo = QIcon(":/icons/edit-redo");
    QIcon iconEditCut = QIcon(":/icons/edit-cut");
    QIcon iconEditCopy = QIcon(":/icons/edit-copy");
    QIcon iconEditPaste = QIcon(":/icons/edit-paste");

    QIcon iconScan = QIcon(":/icons/scan");
    QIcon iconRecurring = QIcon(":/icons/recurring");

    QIcon iconCursorExecute = QIcon(":/icons/cursor-execute");
    QIcon iconPlaybackStart = QIcon(":/icons/playback-start");
    QIcon iconPlaybackPause = QIcon(":/icons/playback-pause");
    QIcon iconSkipBackward = QIcon(":/icons/skip-backward");
    QIcon iconSkipForward = QIcon(":/icons/skip-forward");

    QIcon iconDisplay = QIcon(":/icons/display");
    QIcon iconNetworkConnect = QIcon(":/icons/network-connect");
    QIcon iconNetworkDisconnect = QIcon(":/icons/network-disconnect");

    QIcon iconConfigure = QIcon(":/icons/configure");

    QIcon iconEditor = QIcon(":/icons/editor");
};
