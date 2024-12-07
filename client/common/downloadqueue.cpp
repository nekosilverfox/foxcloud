#include "downloadqueue.h"
#include "common/downloadlayout.h"

DownloadQueue* DownloadQueue::_instance = new DownloadQueue;


/**
 * @brief DownloadQueue::getInstance 获取唯一实例
 * @return
 */
DownloadQueue *DownloadQueue::getInstance()
{
    return _instance;
}

/**
 * @brief DownloadQueue::appendTaskToQueue 向队列中添加下载任务
 * @param cloudFile 需要下载的远端文件信息
 * @param savePath 保存路径（路径+用户名）
 * @return 传输结果状态
 */
TransportStatus DownloadQueue::appendTaskToQueue(const CloudFileInfo* cloudFile, const QString &savePath)
{
    /* 是否已经在队列 */
    for (auto i : _queue)
    {
        if (cloudFile->md5 == i->md5)
        {
            qInfo() << cloudFile->fileName << "already in download queue";
            return TransportStatus::ALREADY_IN_QUEUE;
        }
    }

    QFile* file = new QFile(savePath);
    if (!file->open(QIODevice::WriteOnly))
    {
        qCritical() << "Can not open file" << savePath << file->errorString();
        delete file;
        file = nullptr;
        return TransportStatus::OPEN_FAILED;
    }

    /* 创建下载文件的对象 */
    DownloadFileInfo* downloadFile = new DownloadFileInfo;
    downloadFile->pfile     = file;      // 文件指针
    downloadFile->login     = cloudFile->userLogin;  // 下载用户的 login
    downloadFile->fileName  = cloudFile->fileName;   // 云端该文件的名字
    downloadFile->size      = cloudFile->size;       // 文件大小
    downloadFile->md5       = cloudFile->md5;        // 文件md5码
    downloadFile->url       = cloudFile->url;        // URL
    downloadFile->isDownloading     = false;         // 是否已经在上传
    downloadFile->isDownloaded      = false;         // 是否上传完毕
    downloadFile->isShare   = cloudFile->isShare;    // 是否为共享文件下载

    TransportBar* bar = new TransportBar(nullptr, downloadFile->fileName, downloadFile->size);
    downloadFile->bar = bar;      // 进度控件

    _queue.append(downloadFile);  // 添加到队列
    qDebug() << "Download queue size is" << _queue.size();

    /* 添加到 Upload Layout */
    DownloadLayout::getInstance()->appendTransportBar(downloadFile->bar);

    qInfo() << "Successful append download task, File:" << downloadFile->fileName;
    return TransportStatus::SUCCESS;
}

/**
 * @brief DownloadQueue::getFileToDownload 从队列中获取一个文件去下载
 * @return 需要下载的文件
 */
DownloadFileInfo *DownloadQueue::getFileToDownload()
{
    if (_queue.isEmpty())
    {
        return nullptr;
    }

    //TODO 多线程下载这里要改掉
    DownloadFileInfo* task = _queue.at(0);
    task->isDownloading = true;

    return task;
}

/**
 * @brief DownloadQueue::isQueueEmpty 队列是否为空
 * @return
 */
bool DownloadQueue::isQueueEmpty()
{
    return _queue.isEmpty();
}

/**
 * @brief DownloadQueue::isDownloading 队列中是否有文件在下载
 * @return
 */
bool DownloadQueue::isDownloading()
{
    for (DownloadFileInfo* i :_queue)
    {
        if (i->isDownloading)
        {
            return true;
        }
    }

    return false;
}

/**
 * @brief DownloadQueue::removeFinsishedTask 删除已经完成下载的文件
 */
void DownloadQueue::removeFinsishedTask()
{
    for (int i = 0; i < _queue.size(); i++)
    {
        if (!_queue.at(i)->isDownloaded)
        {
            continue;
        }

        DownloadFileInfo* task = _queue.takeAt(i);
        qInfo() << "Remove task" << task->fileName << "from download queue";
        if (nullptr != task->pfile)
        {
            task->pfile->close();
            delete task->pfile;
        }

        DownloadLayout::getInstance()->removeTransportBar(task->bar);
        delete task->bar;

        delete task;
    }
}

/**
 * @brief DownloadQueue::clearQueue 清空下载队列（删除所有）
 */
void DownloadQueue::clearQueue()
{
    while (!_queue.isEmpty())
    {
        DownloadFileInfo* cur = _queue.takeFirst();
        if (nullptr != cur->pfile)
        {
            cur->pfile->close();
            delete cur;
        }

        delete cur->bar;
    }
}

DownloadQueue::DownloadQueue() {}

DownloadQueue::~DownloadQueue() {}
