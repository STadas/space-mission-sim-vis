#pragma once

#include <QtConcurrent>
#include <QtCore>

#include "common/MessageController.hpp"
#include "common/PanguConnection.hpp"
#include "enums/ConnectionErr.hpp"
#include "util/CommandUtil.hpp"
#include "util/ThreadUtil.hpp"
#include "widgets/SimPreview.hpp"

class PanguWorker : public QObject
{
    Q_OBJECT

public:
    PanguWorker();
    ~PanguWorker();

    QSemaphore *previewLock;

    void setCancelled(bool cancelled);

private:
    PanguConnection *connection_;

    bool isCancelled_;

    void linePreReturn(int currLine, int toLine, int msDelay, CommandErr err);

signals:
    void error(CommandErr err);
    void error(ConnectionErr err);

    void askLine(int fromLine, int toLine = 0, int msDelay = 0);
    void giveLine(QString lineStr, int fromLine = 0, int toLine = 0, int msDelay = 0);

    void changePreview(unsigned char *img, unsigned long size);

    void doMultiLine(int fromLine, int toLine, int msDelay);
    void stopMultiLine();

    void lineDone();
    void multiLineDone();

private slots:
    void onGiveLine(QString lineStr, int currLine, int toLine, int msDelay);

    void onStopMultiLine();
};
