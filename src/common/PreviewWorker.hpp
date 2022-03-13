#pragma once

#include <QtConcurrent>
#include <QtCore>

#include "common/PanguConnection.hpp"
#include "common/PanguParser.hpp"
#include "enums/CommandErr.hpp"
#include "enums/ConnectionErr.hpp"
#include "interfaces/IConnection.hpp"
#include "util/ThreadUtil.hpp"

class PreviewWorker : public QObject, IConnection
{
    Q_OBJECT

public:
    PreviewWorker();
    ~PreviewWorker();

    QSemaphore *previewLock() const;
    std::vector<int> imgIndices() const;

private:
    PanguConnection *connection_;
    PanguParser *parser_;

    QSemaphore *previewLock_;
    bool isCancelled_;
    std::vector<int> imgIndices_;

signals:
    ConnectionErr connect(const QString &address, const int &port) override;
    ConnectionErr disconnect() override;

    void error(CommandErr err);
    void error(ConnectionErr err);

    void processText(const QString &text, const int &msLineDelay = 0,
                     const int &start = 0);
    void lineStarted(const int &lineNum);
    void changePreview(QByteArray byteArr, unsigned long size);
    void cancel();

    void textProcessed();

    void updateImgIndices(const QString &str);
    void imgIndicesUpdated();

private slots:
    ConnectionErr onConnect(const QString &address, const int &port);
    ConnectionErr onDisconnect();

    void onProcessText(const QString &text, const int &msLineDelay = 0,
                       const int &start = 0);

    void onCancel();

    void onUpdateImgIndices(const QString &str);
};
