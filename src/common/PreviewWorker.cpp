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
    QObject::connect(this, &PreviewWorker::processCommands, this,
                     &PreviewWorker::onProcessCommands);
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

void PreviewWorker::onProcessCommands(const QString &text, const int &start,
                                      const int &msLineDelay)
{
    this->previewLock_->acquire();

    QStringList lines = text.split(QRegExp("\\n"));

    int end = lines.size();
    if (end == 1)
        end = start + 1;

    for (int i = start; i < end; i++)
    {
        QTime startTime = QTime::currentTime();

        if (lines.size() > 1)
            emit this->lineStarted(i);

        ParseResult parsed = this->parser_->parse(lines[i]);

        if (parsed.err != CommandErr::OK)
        {
            emit this->error(parsed.err);

            if (parsed.err == CommandErr::EMPTY)
                continue;
            else
                break;
        }
        if (!parsed.command.has_value())
        {
            emit this->error(CommandErr::UNKNOWN);
            break;
        }

        unsigned char *img = nullptr;
        unsigned long size{};
        ConnectionErr connectionErr =
            this->connection_->sendCommand(parsed.command.value(), img, size);

        if (connectionErr != ConnectionErr::OK)
        {
            emit this->error(connectionErr);
            if (img != nullptr)
                delete img;
            break;
        }

        if (!parsed.command->expectsImg() && img != nullptr)
            delete img;

        if (parsed.command->expectsImg())
        {
            QByteArray data(reinterpret_cast<char *>(img), size);
            delete img;

            emit this->changePreview(data, size);
        }

        if (this->isCancelled_)
            break;

        if (parsed.err != CommandErr::EMPTY && i < lines.size() - 1)
        {
            // If the command itself took longer to execute than the specified
            // delay, don't sleep.
            unsigned int msDiff = startTime.msecsTo(QTime::currentTime());
            QThread::currentThread()->msleep(
                std::max<int>(msLineDelay - msDiff, 0));
        }
    }

    emit this->commandsProcessed();
    this->isCancelled_ = false;
    this->previewLock()->release();
}

void PreviewWorker::cancelStepping()
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
    QStringList lines = str.split(QRegExp("\\n"));

    std::vector<int> newImgIndices;
    int lineNum = 0;

    for (auto &line : lines)
    {
        ParseResult parsed = this->parser_->parse(line);

        if (parsed.err == CommandErr::OK && parsed.command->expectsImg())
        {
            newImgIndices.push_back(lineNum);
        }

        lineNum++;
    }
    this->imgIndices_ = newImgIndices;
    emit this->imgIndicesUpdated();
}
