#pragma once

#include <QtConcurrent>
#include <QtCore>

#include "common/CamPoint.hpp"
#include "common/PanguConnection.hpp"
#include "common/PanguParser.hpp"
#include "common/settings/Settings.hpp"
#include "enums/CommandErr.hpp"
#include "enums/ConnectionErr.hpp"
#include "interfaces/IConnection.hpp"
#include "util/ThreadUtil.hpp"

/**
 * Preview worker class. Should be used in a separate thread. Uses
 * PanguConnection to do background tasks such as connecting/disconnecting,
 * sending commands, etc.
 */
class PreviewWorker : public QObject, public IConnection
{
    Q_OBJECT

public:
    /**
     * Constructor
     * @param settings Settings object
     */
    PreviewWorker(Settings *const settings);
    /**
     * Destructor
     */
    ~PreviewWorker();

    /**
     * @return Preview lock semaphore
     */
    QSemaphore *previewLock() const;
    /**
     * @return Camera points list
     */
    QList<CamPoint> camPoints() const;
    /**
     * Cancel multi-command stepping
     */
    void cancelStepping();

signals:
    /**
     * @see PreviewWorker::onConnect
     */
    ConnectionErr connect(const QString &address, const int &port) override;
    /**
     * @see PreviewWorker::onDisconnect
     */
    ConnectionErr disconnect() override;

    /**
     * @see PreviewWorker::onPing
     * */
    ConnectionErr ping() override;

    /**
     * Forwards PanguConnection::connected
     */
    void connected() override;
    /**
     * Forwards PanguConnection::disconnected
     */
    void disconnected() override;

    /**
     * Emitted on command error
     * @param err Error status
     */
    void error(CommandErr err);
    /**
     * Emitted on connection error
     * @param err Error status
     */
    void error(ConnectionErr err);

    /**
     * @see PreviewWorker::onProcessCommands
     */
    void processCommands(const QString &text, const int &start = 0,
                         const int &msLineDelay = 0);
    /**
     * Emitted when a line has started being processed
     * @param lineNum Line number that will be processed
     */
    void lineStarted(const unsigned int &lineNum);
    /**
     * Emitted when an image has been received from the server
     * @param byteArr Image data
     */
    void changePreview(QByteArray byteArr);

    /**
     * Emitted when all of the commands in the given text have been processed
     */
    void commandsProcessed();

    /**
     * @see PreviewWorker::updateCamPoints
     */
    void updateCamPoints(const QString &str);
    /**
     * Emitted when the camera points list has been updated
     */
    void camPointsUpdated();

private slots:
    /**
     * Connect to a server
     * @param address Address of the server
     * @param port Port of the server
     * @return Error status
     * @see PreviewWorker::connect
     */
    ConnectionErr onConnect(const QString &address, const int &port);
    /**
     * Disconnect from the current server
     * @return Error status
     * @see PreviewWorker::disconnect
     */
    ConnectionErr onDisconnect();
    /**
     * Ping the current server
     * @return Error status
     * @see PreviewWorker::ping
     */
    ConnectionErr onPing();

    /**
     * Parses and sends commands from text
     * @param text Text to parse commands from
     * @param start Line number to start from
     * @param msLineDelay Delay in milliseconds between each command
     * @see PreviewWorker::processCommands
     */
    void onProcessCommands(const QString &text, const int &start = 0,
                           const int &msLineDelay = 0);

    /**
     * Updates the camera points list
     * @param str Text to parse commands from
     * @see PreviewWorker::updateCamPoints
     */
    void onUpdateCamPoints(const QString &str);

private:
    Settings *const settings_;

    PanguConnection *connection_;
    PanguParser *parser_;

    QList<CamPoint> camPoints_;

    QSemaphore *previewLock_;
    std::atomic<bool> isCancelled_;
};
