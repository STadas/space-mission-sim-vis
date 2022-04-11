#pragma once

#include <Qt3DRender/QShaderProgram>
#include <QtCore>
#include <QtWidgets>

/**
 * Resources class. Stores all compiled resources
 */
class Resources : public QObject
{
    Q_OBJECT

public:
    /**
     * Constructor
     * @param parent Parent object
     */
    Resources(QObject *parent);
    /**
     * Destructor
     */
    ~Resources();

    QUrl manualUrl = QUrl("https://github.com/STadas/space-mission-sim-vis/"
                          "blob/main/SpaceMissionSimVis_User_Manual.pdf");

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
    QIcon iconCoordinate = QIcon(":/icons/coordinate");

    QIcon iconOk = QIcon(":/icons/ok");
    QIcon iconWarn = QIcon(":/icons/warn");

    QIcon iconQuestion = QIcon(":/icons/question");
    QIcon iconInfo = QIcon(":/icons/info");

    QByteArray flightPathVShader = Qt3DRender::QShaderProgram::loadSource(
        QUrl("qrc:/shaders/FlightPathV"));
    QByteArray flightPathFShader = Qt3DRender::QShaderProgram::loadSource(
        QUrl("qrc:/shaders/FlightPathF"));
};
