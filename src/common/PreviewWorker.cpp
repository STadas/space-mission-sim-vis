#include "PreviewWorker.hpp"

PreviewWorker::PreviewWorker()
    : QObject(nullptr)
    , connection_(new PanguConnection(this))
    , parser_(new PanguParser(this))
    , previewLock_(new QSemaphore(1))
    , isCancelled_(false)
{
    QObject::connect(this, &PreviewWorker::connect, this,
                     &PreviewWorker::onConnect);
    QObject::connect(this, &PreviewWorker::disconnect, this,
                     &PreviewWorker::onDisconnect);
    QObject::connect(this, &PreviewWorker::processText, this,
                     &PreviewWorker::onProcessText);
    QObject::connect(this, &PreviewWorker::cancel, this,
                     &PreviewWorker::onCancel);
    QObject::connect(this, &PreviewWorker::updateImgIndices, this,
                     &PreviewWorker::onUpdateImgIndices);
}

PreviewWorker::~PreviewWorker()
{
}

QSemaphore *PreviewWorker::previewLock() const
{
    return this->previewLock_;
}

std::vector<int> PreviewWorker::imgIndices() const
{
    return this->imgIndices_;
}

ConnectionErr PreviewWorker::onConnect(const QString &address, const int &port)
{
    return this->connection_->connect(address, port);
}

ConnectionErr PreviewWorker::onDisconnect()
{
    return this->connection_->disconnect();
}

void PreviewWorker::onProcessText(const QString &text, const int &msLineDelay,
                                  const int &start)
{
    this->previewLock_->acquire();

    QStringList lines = StringUtil::split(text, "\\n");

    for (int i = start; i < lines.size(); i++)
    {
        QString &line = lines[i];

        if (lines.size() > 1)
            emit this->lineStarted(i);

        std::unique_ptr<ParsedCommand> parsedCommand;
        CommandErr commandErr = this->parser_->parse(line, parsedCommand);

        if (commandErr != CommandErr::OK)
        {
            emit this->error(commandErr);

            if (commandErr != CommandErr::EMPTY)
                break;
        }

        unsigned char *img = nullptr;
        unsigned long size{};
        ConnectionErr connectionErr =
            this->connection_->sendCommand(parsedCommand, img, size);

        if (connectionErr != ConnectionErr::OK)
        {
            emit this->error(connectionErr);
            break;
        }

        if (!parsedCommand->expectsImg() && img != nullptr)
            delete img;

        if (parsedCommand->expectsImg() && img == nullptr)
        {
            emit this->error(ConnectionErr::BAD_DATA);
            break;
        }

        QByteArray data(reinterpret_cast<char *>(img), size);
        delete img;

        emit this->changePreview(data, size);

        if (this->isCancelled_)
            break;

        if (commandErr != CommandErr::EMPTY && lines.size() > 1)
            QThread::currentThread()->msleep(msLineDelay);
    }

    emit this->textProcessed();
    this->isCancelled_ = false;
    this->previewLock()->release();
}

void PreviewWorker::onCancel()
{
    if (!this->previewLock_->available())
    {
        this->isCancelled_ = true;
    }
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
