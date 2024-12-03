#ifndef UPLOADQUEUE_H
#define UPLOADQUEUE_H

#include <QList>

#include "structs/fileinfo.h"


/**
 * @brief 上传文件队列的单例类
 */
class UploadQueue
{
public:
    static UploadQueue* getInstance();  // 约定俗成：构造函数私有化并且带有公有方法 getInstance 的基本都是单例模式的实现，用于获取唯一实例

    TransportStatus appendUploadFile(const QString& filePath);

    UploadFileInfo* getFileToUpload();

    bool isQueueEmpty();
    bool isUploading();

    void removeFinsishedTask();
    void clearQueue();

private:
    UploadQueue();
    ~UploadQueue();

private:
    static UploadQueue*    _instance;  // 本类唯一单例实例
    QList<UploadFileInfo*> _queue;
};

#endif // UPLOADQUEUE_H
