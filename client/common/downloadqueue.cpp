#include "downloadqueue.h"

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
 * @param isShare   是否为分享文件
 * @return 传输结果状态
 */
TransportStatus DownloadQueue::appendTaskToQueue(const CloudFileInfo *cloudFile, const QString &savePath, bool isShare)
{

}

/**
 * @brief DownloadQueue::getFileToDownload 从队列中获取一个文件去下载
 * @return 需要下载的文件
 */
DownloadFileInfo *DownloadQueue::getFileToDownload()
{

}

/**
 * @brief DownloadQueue::isQueueEmpty 队列是否为空
 * @return
 */
bool DownloadQueue::isQueueEmpty()
{

}

/**
 * @brief DownloadQueue::isDownloading 队列中是否有文件在下载
 * @return
 */
bool DownloadQueue::isDownloading()
{

}

/**
 * @brief DownloadQueue::removeFinsishedTask 删除已经完成下载的文件
 */
void DownloadQueue::removeFinsishedTask()
{

}

/**
 * @brief DownloadQueue::clearQueue 清空下载队列（删除所有）
 */
void DownloadQueue::clearQueue()
{

}
