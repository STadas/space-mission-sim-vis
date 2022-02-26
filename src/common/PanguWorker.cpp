#include "PanguWorker.hpp"
#include "common/MessageController.hpp"

PanguWorker::PanguWorker()
    : QObject(nullptr)
    , connection_(new PanguConnection(this))
    , previewLock(new QSemaphore(1))
{
    this->connection_->connect();

    connect(this, &PanguWorker::giveLine, this, &PanguWorker::onGiveLine);
    connect(this, &PanguWorker::doMultiLine, this, &PanguWorker::askLine);
    connect(this, &PanguWorker::stopMultiLine, this,
            &PanguWorker::onStopMultiLine);
}

PanguWorker::~PanguWorker()
{
}

void PanguWorker::setCancelled(bool cancelled)
{
    this->isCancelled_ = cancelled;
}

void PanguWorker::linePreReturn(int currLine, int toLine, int msDelay, CommandErr err)
{
    if (currLine == toLine)
    {
        emit lineDone();
        this->previewLock->release();
        return;
    }

    if (currLine == toLine - 1 || (currLine < toLine && this->isCancelled_))
    {
        emit lineDone();
        emit this->multiLineDone();
        this->previewLock->release();
        return;
    }

    if (err != CommandErr::EMPTY)
        QThread::currentThread()->msleep(msDelay);

    emit lineDone();
    this->previewLock->release();
    emit this->askLine(currLine + 1, toLine, msDelay);
}

void PanguWorker::onGiveLine(QString lineStr, int currLine, int toLine,
                             int msDelay)
{
    this->previewLock->acquire();

    std::unique_ptr<ParsedCommand> parsedCommand;

    CommandErr commandErr = CommandUtil::parsePangu(lineStr, parsedCommand);

    if (commandErr != CommandErr::OK)
    {
        emit this->error(commandErr);

        if (commandErr == CommandErr::EMPTY)
            return this->linePreReturn(currLine, toLine, msDelay, commandErr);
    }

    unsigned char *img = nullptr;
    unsigned long size{};
    ConnectionErr connectionErr =
        this->connection_->sendCommand(parsedCommand, img, size);

    if (connectionErr != ConnectionErr::OK)
    {
        emit this->error(connectionErr);
        return;
    }

    if (!parsedCommand->expectsImage() && img != nullptr)
        delete img;

    emit this->changePreview(img, size);

    return this->linePreReturn(currLine, toLine, msDelay, commandErr);
}

void PanguWorker::onStopMultiLine()
{
    this->setCancelled(true);
}
