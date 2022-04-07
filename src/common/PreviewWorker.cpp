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
    QObject::connect(this, &PreviewWorker::ping, this, &PreviewWorker::onPing);

    QObject::connect(this, &PreviewWorker::processCommands, this,
                     &PreviewWorker::onProcessCommands);
    QObject::connect(this, &PreviewWorker::updateCamPoints, this,
                     &PreviewWorker::onUpdateCamPoints);

    QObject::connect(this->connection_, &PanguConnection::connected, this,
                     &PreviewWorker::connected);
    QObject::connect(this->connection_, &PanguConnection::disconnected, this,
                     &PreviewWorker::disconnected);
}

PreviewWorker::~PreviewWorker()
{
}

QSemaphore *PreviewWorker::previewLock() const
{
    return this->previewLock_;
}

QList<CamPoint> PreviewWorker::camPoints() const
{
    return this->camPoints_;
}

ConnectionErr PreviewWorker::onConnect(const QString &address, const int &port)
{
    return this->connection_->connect(address, port);
}

ConnectionErr PreviewWorker::onDisconnect()
{
    return this->connection_->disconnect();
}

ConnectionErr PreviewWorker::onPing()
{
    if (this->previewLock_->tryAcquire())
    {
        ConnectionErr err = this->connection_->ping();
        this->previewLock_->release();
        return err;
    }

    return ConnectionErr::Unknown;
}

void PreviewWorker::onProcessCommands(const QString &text, const int &start,
                                      const int &msLineDelay)
{
    this->previewLock_->acquire();

    QStringList lines = text.split(QRegExp("\\n"));

    int end = lines.size();
    if (end == 1)
        end = start + 1;

    for (int i = start; i < end; ++i)
    {
        QTime startTime = QTime::currentTime();

        if (lines.size() > 1)
            emit this->lineStarted(i);

        ParseResult parsed = this->parser_->parse(lines[i]);

        if (parsed.err != CommandErr::Ok)
        {
            emit this->error(parsed.err);

            if (parsed.err == CommandErr::Empty)
                continue;
            else
                break;
        }
        if (!parsed.command.has_value())
        {
            emit this->error(CommandErr::Unknown);
            break;
        }

        unsigned char *img = nullptr;
        unsigned long size{};
        ConnectionErr connectionErr =
            this->connection_->sendCommand(parsed.command.value(), img, size);

        if (connectionErr != ConnectionErr::Ok)
        {
            emit this->error(connectionErr);
            if (img != nullptr)
                delete img;
            break;
        }

        emit this->connected();

        if (!parsed.command->expectsImg() && img != nullptr)
            delete img;

        if (parsed.command->expectsImg())
        {
            QByteArray data(reinterpret_cast<char *>(img), size);
            delete img;

            emit this->changePreview(data);
        }

        if (this->isCancelled_)
            break;

        if (parsed.err != CommandErr::Empty && i < lines.size() - 1)
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

void PreviewWorker::onUpdateCamPoints(const QString &str)
{
    /* TODO: Some kind of progress bar in the statusbar would be good for large
     * files */
    QStringList lines = str.split(QRegExp("\\n"));

    QList<CamPoint> newCamPoints;
    unsigned int lineNum = 0;
    QVector3D lastCamPos = {0, 0, 0};

    for (auto &line : lines)
    {
        ParseResult parsed = this->parser_->parse(line);

        if (parsed.err == CommandErr::Ok && parsed.command->expectsImg())
        {
            if (parsed.command->hasCamPos())
            {
                newCamPoints.push_back({lineNum, parsed.command->camPos()});
            }
            else
                newCamPoints.push_back({lineNum, lastCamPos});
        }

        lineNum++;
    }
    this->camPoints_ = newCamPoints;
    emit this->camPointsUpdated();
}
