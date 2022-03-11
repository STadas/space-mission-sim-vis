#include "PreviewWorker.hpp"

PreviewWorker::PreviewWorker()
    : QObject(nullptr)
    , connection_(new PanguConnection(this))
    , parser_(new PanguParser(this))
    , previewLock_(new QSemaphore(1))
    , isCancelled_(false)
{
    QObject::connect(this, &PreviewWorker::giveLine, this,
                     &PreviewWorker::onGiveLine);
    QObject::connect(this, &PreviewWorker::doMultiLine, this,
                     &PreviewWorker::askLine);
    QObject::connect(this, &PreviewWorker::stopMultiLine, this,
                     &PreviewWorker::onStopMultiLine);
    QObject::connect(this, &PreviewWorker::updateImgIndices, this,
                     &PreviewWorker::onUpdateImgIndices);
    QObject::connect(this, &PreviewWorker::connect, this,
                     &PreviewWorker::onConnect);
    QObject::connect(this, &PreviewWorker::disconnect, this,
                     &PreviewWorker::onDisconnect);
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

std::vector<int> PreviewWorker::imgIndices() const
{
    return this->imgIndices_;
}

void PreviewWorker::linePreReturn(int currLine, int toLine, int msDelay,
                                  CommandErr err)
{
    /* Only one line is being done */
    if (currLine >= toLine)
    {
        emit lineDone();
        this->previewLock()->release();
        this->setCancelled(false);
        return;
    }

    /* Multiple lines are being done. Check if last one or cancelled. */
    if (currLine == toLine - 1 || (currLine < toLine && this->isCancelled_))
    {
        emit lineDone();
        emit this->multiLineDone();
        this->previewLock()->release();
        this->setCancelled(false);
        return;
    }

    if (err != CommandErr::EMPTY)
        QThread::currentThread()->msleep(msDelay);

    emit lineDone();
    this->previewLock()->release();
    emit this->askLine(currLine + 1, toLine, msDelay);
}

ConnectionErr PreviewWorker::onConnect(const QString &address, const int &port)
{
    return this->connection_->connect(address, port);
}

ConnectionErr PreviewWorker::onDisconnect()
{
    return this->connection_->disconnect();
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

    if (!parsedCommand->expectsImg() && img != nullptr)
        delete img;

    emit this->changePreview(img, size);

    return this->linePreReturn(currLine, toLine, msDelay, commandErr);
}

void PreviewWorker::onStopMultiLine()
{
    this->setCancelled(true);
}

void PreviewWorker::onUpdateImgIndices(const QString &str)
{
    /* TODO: Some kind of progress bar in the statusbar would be good for large
     * files */
    QStringList lines = StringUtil::split(str, "\\n");
    std::vector<int> newImgIndices;
    int lineNum = 0;

    for (auto &line : lines)
    {
        std::unique_ptr<ParsedCommand> parsedCommand;
        CommandErr err = this->parser_->parse(line, parsedCommand);

        if (err == CommandErr::OK && parsedCommand->expectsImg())
        {
            newImgIndices.push_back(lineNum);
        }

        lineNum++;
    }
    this->imgIndices_ = newImgIndices;
    emit this->imgIndicesUpdated();
}
