#pragma once

#include <QtConcurrent>
#include <QtCore>

#include "common/CamPoint.hpp"
#include "common/PanguConnection.hpp"
#include "common/PanguParser.hpp"
#include "enums/CommandErr.hpp"
#include "enums/ConnectionErr.hpp"
#include "interfaces/IConnection.hpp"
#include "util/ThreadUtil.hpp"

class PreviewWorker : public QObject, public IConnection
{
    Q_OBJECT

public:
    PreviewWorker();
    ~PreviewWorker();

    QSemaphore *previewLock() const;
    QList<CamPoint> camPoints() const;
    void cancelStepping();

private:
    PanguConnection *connection_;
    PanguParser *parser_;

    QList<CamPoint> camPoints_;

    QSemaphore *previewLock_;
    std::atomic<bool> isCancelled_;

signals:
    ConnectionErr connect(const QString &address, const int &port) override;
    ConnectionErr disconnect() override;

    ConnectionErr ping() override;

    void connected() override;
    void disconnected() override;

    void error(CommandErr err);
    void error(ConnectionErr err);

    void processCommands(const QString &text, const int &start = 0,
                         const int &msLineDelay = 0);
    void lineStarted(const int &lineNum);
    void changePreview(QByteArray byteArr, unsigned long size);

    void commandsProcessed();

    void updateCamPoints(const QString &str);
    void camPointsUpdated();

private slots:
    ConnectionErr onConnect(const QString &address, const int &port);
    ConnectionErr onDisconnect();
    ConnectionErr onPing();

    void onProcessCommands(const QString &text, const int &start = 0,
                           const int &msLineDelay = 0);

    void onUpdateCamPoints(const QString &str);
};
