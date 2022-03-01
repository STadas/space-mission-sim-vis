#pragma once

#include <QtConcurrent>
#include <QtCore>

#include "common/PanguConnection.hpp"
#include "common/PanguParser.hpp"
#include "enums/CommandErr.hpp"
#include "enums/ConnectionErr.hpp"
#include "util/ThreadUtil.hpp"

class PreviewWorker : public QObject
{
    Q_OBJECT

public:
    PreviewWorker();
    ~PreviewWorker();

    void setCancelled(const bool &cancelled);

    QSemaphore *previewLock() const;
    std::vector<int> imageIndexes() const;

private:
    PanguConnection *connection_;
    PanguParser *parser_;

    QSemaphore *previewLock_;
    bool isCancelled_;
    bool processingPreview_;
    std::vector<int> imageIndexes_;

    void linePreReturn(int currLine, int toLine, int msDelay, CommandErr err);

signals:
    void error(CommandErr err);
    void error(ConnectionErr err);

    void askLine(int fromLine, int toLine = 0, int msDelay = 0);
    void giveLine(QString lineStr, int fromLine = 0, int toLine = 0,
                  int msDelay = 0);

    void updateImageIndexes(const QString &str);
    void imageIndexesUpdated();
    void changePreview(unsigned char *img, unsigned long size);

    void doMultiLine(int fromLine, int toLine, int msDelay);
    void stopMultiLine();

    void lineDone();
    void multiLineDone();

private slots:
    void onGiveLine(QString lineStr, int currLine, int toLine, int msDelay);

    void onUpdateImageIndexes(const QString &str);

    void onStopMultiLine();
};
