#ifndef DOWNLOADQUEUE_H
#define DOWNLOADQUEUE_H

#include <QList>

#include "structs/fileinfo.h"


/**
 * @brief 上传队列
 */
class DownloadQueue
{
public:
    static DownloadQueue* getInstance();

    TransportStatus appendTaskToQueue(const CloudFileInfo* cloudFile, const QString& savePath);
    DownloadFileInfo* getFileToDownload();

    bool isQueueEmpty();
    bool isDownloading();

    void removeFinsishedTask();
    void clearQueue();


private:
    DownloadQueue();
    ~DownloadQueue();

private:
    static DownloadQueue* _instance;
    QList<DownloadFileInfo*> _queue;
};

#endif // DOWNLOADQUEUE_H
