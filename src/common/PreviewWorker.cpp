#include "PreviewWorker.hpp"

PreviewWorker::PreviewWorker()
    : QObject(nullptr)
    , connection_(new PanguConnection(this))
    , parser_(new PanguParser(this))
    , previewLock_(new QSemaphore(1))
    , isCancelled_(false)
    , processingPreview_(false)
{
    this->connection_->connect();

    connect(this, &PreviewWorker::giveLine, this, &PreviewWorker::onGiveLine);
    connect(this, &PreviewWorker::doMultiLine, this, &PreviewWorker::askLine);
    connect(this, &PreviewWorker::stopMultiLine, this,
            &PreviewWorker::onStopMultiLine);
    connect(this, &PreviewWorker::updateImageIndexes, this,
            &PreviewWorker::onUpdateImageIndexes);
}

PreviewWorker::~PreviewWorker()
{
}

void PreviewWorker::setCancelled(const bool &cancelled)
{
    this->isCancelled_ = cancelled;
}

QSemaphore *PreviewWorker::previewLock() const
{
    return this->previewLock_;
}

std::vector<int> PreviewWorker::imageIndexes() const
{
    return this->imageIndexes_;
}

void PreviewWorker::onUpdateImageIndexes(const QString &str)
{
    std::vector<QString> lines = StringUtil::split(str, "\\n");
    std::vector<int> newImageIndexes;
    int lineNum = 0;

    for (auto &line : lines)
    {
        std::unique_ptr<ParsedCommand> parsedCommand;
        CommandErr err = this->parser_->parse(line, parsedCommand);

        if (err == CommandErr::OK && parsedCommand->expectsImage())
        {
            newImageIndexes.push_back(lineNum);
        }

        lineNum++;
    }
    this->imageIndexes_ = newImageIndexes;
    emit this->imageIndexesUpdated();
}

void PreviewWorker::linePreReturn(int currLine, int toLine, int msDelay,
                                  CommandErr err)
{
    /* Only one line is being done */
    if (currLine >= toLine)
    {
        emit lineDone();
        this->previewLock()->release();
        return;
    }

    /* Multiple lines are being done. Check if last one or cancelled. */
    if (currLine == toLine - 1 || (currLine < toLine && this->isCancelled_))
    {
        emit lineDone();
        emit this->multiLineDone();
        this->previewLock()->release();
        return;
    }

    if (err != CommandErr::EMPTY)
        QThread::currentThread()->msleep(msDelay);

    emit lineDone();
    this->previewLock()->release();
    emit this->askLine(currLine + 1, toLine, msDelay);
}

void PreviewWorker::onGiveLine(QString lineStr, int currLine, int toLine,
                               int msDelay)
{
    this->previewLock()->acquire();

    std::unique_ptr<ParsedCommand> parsedCommand;

    CommandErr commandErr = this->parser_->parse(lineStr, parsedCommand);

    if (commandErr != CommandErr::OK)
    {
        if (commandErr != CommandErr::EMPTY)
            this->setCancelled(true);

        emit this->error(commandErr);
        return this->linePreReturn(currLine, toLine, msDelay, commandErr);
    }

    unsigned char *img = nullptr;
    unsigned long size{};
    ConnectionErr connectionErr =
        this->connection_->sendCommand(parsedCommand, img, size);

    if (connectionErr != ConnectionErr::OK)
    {
        emit this->error(connectionErr);
        this->setCancelled(true);
        return this->linePreReturn(currLine, toLine, msDelay, commandErr);
    }

    if (!parsedCommand->expectsImage() && img != nullptr)
        delete img;

    emit this->changePreview(img, size);

    return this->linePreReturn(currLine, toLine, msDelay, commandErr);
}

void PreviewWorker::onStopMultiLine()
{
    this->setCancelled(true);
}
