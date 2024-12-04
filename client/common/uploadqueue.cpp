#include "uploadqueue.h"

#include <QFile>
#include <QFileInfo>

#include "common/encrypttool.h"
#include "common/uploadlayout.h"
#include "transportbar.h"

/* 初始化变量 */
UploadQueue* UploadQueue::_instance = new UploadQueue;

UploadQueue::UploadQueue() {}


/**
 * @brief UploadQueue::getInstance 获取唯一实例
 * @return 任务队列实例
 */
UploadQueue* UploadQueue::getInstance()
{
    return _instance;
}

/**
 * @brief UploadQueue::appendUploadFile 添加文件到上传队列
 * @param filePath
 * @return
 */
TransportStatus UploadQueue::appendUploadFile(const QString& filePath)
{
    /* 是否已经在队列 */
    for (auto i : _queue)
    {
        if (filePath == i->path)
        {
            qWarning() << "File" << filePath << "aleady in upload queue";
            return TransportStatus::ALREADY_IN_QUEUE;
        }
    }

    QFile* file = new QFile(filePath);
    if (!file->open(QIODevice::ReadOnly))
    {
        qCritical() << "Can not open file" << filePath << file->errorString();
        delete file;
        file = nullptr;
        return TransportStatus::OPEN_FAILED;
    }

    /* 创建上传文件的对象 */
    QFileInfo fileInfo(filePath);
    UploadFileInfo* uploadFile =  new UploadFileInfo;
    uploadFile->pfile = file;
    uploadFile->name  = fileInfo.fileName();
    uploadFile->path  = fileInfo.filePath();
    uploadFile->size  = fileInfo.size();
    uploadFile->md5   = EncryptTool::getFileMD5(uploadFile->path);
    uploadFile->bar   = new TransportBar(nullptr, uploadFile->name, uploadFile->size);
    uploadFile->isUploaded = false;
    uploadFile->isUploading = false;

    _queue.append(uploadFile);

    /* 添加到 Upload Layout */
    UploadLayout::getInstance()->appendTransportBar(uploadFile->bar);

    qInfo() << "Successful append upload task, File:" << uploadFile->path;
    return TransportStatus::SUCCESS;
}

/** TODO 如果多任务上传这里需要修改获取方式
 * @brief UploadQueue::getFileToUpload 获取一个需要上传的文件（队列第 0 个文件），如果没有任务上传，设置第 0 个上传
 * @return 队列中第一个任务，如果队列为空则返回 nullptr
 */
UploadFileInfo* UploadQueue::getFileToUpload()
{
    if (_queue.isEmpty())
    {
        return nullptr;
    }

    UploadFileInfo* task = _queue.at(0);
    task->isUploading = true;

    return task;
}

/**
 * @brief UploadQueue::isQueueEmpty 上传队列是否为空
 * @return
 */
bool UploadQueue::isQueueEmpty()
{
    return _queue.isEmpty();
}

/**
 * @brief UploadQueue::isUploading 是否有任务在上传
 * @return
 */
bool UploadQueue::isUploading()
{
    for (auto i : _queue)
    {
        if (i->isUploading)
        {
            return true;
        }
    }

    return false;
}


/**
 * @brief UploadQueue::removeFinsishedTask 从队列中移除传输完成的任务
 */
void UploadQueue::removeFinsishedTask()
{
    for (int i = 0; i < _queue.size(); i++)
    {
        if (_queue.at(i)->isUploaded)
        {
            UploadFileInfo* curTask = _queue.takeAt(i);

            UploadLayout::getInstance()->removeTransportBar(curTask->bar);  // 移除布局
            delete curTask->bar;

            curTask->pfile->close();
            delete curTask->pfile;

            delete curTask;
        }
    }

    return;
}

/**
 * @brief UploadQueue::clearQueue 清空任务队列（移除所有任务）
 */
void UploadQueue::clearQueue()
{
    for (int i = 0; i < _queue.size(); i++)
    {
        delete _queue.takeFirst();
    }

}

