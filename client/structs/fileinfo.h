#ifndef FILEINFO_H
#define FILEINFO_H


#include <QFile>
#include <QListWidgetItem>

#include "transportbar.h"


/**
 * @brief 传输错误状态
 */
enum TransportStatus
{
    SUCCESS,
    FILE_TOO_BIG,       // 文件过大
    ALREADY_IN_QUEUE,   // 文件已经在传输队列
    OPEN_FAILED,        // 打开文件失败
    GET_LAYOUT_FAILED   // 获取布局失败
};

/* 要上传文件信息 */
struct UploadFileInfo
{
    QFile*  pfile;      // 文件指针
    QString md5;        // 文件md5码
    QString name;       // 文件名字
    QString path;       // 文件路径
    qint64  size;       // 文件大小
    bool    isUploading;// 是否已经在上传
    bool    isUploaded; // 是否上传完毕
    TransportBar* bar;  // 上传进度控件
};

/* 要上传文件信息 */
struct DownloadFileInfo
{
    QFile*      pfile;      // 文件指针
    QString     login;      // 下载用户的 login
    QString     fileName;   // 文件名字
    QString     md5;        // 文件md5码
    QString     url;        // URL
    bool        isDownloading;// 是否已经在上传
    bool        isDownloaded; // 是否上传完毕
    bool        isShare;    // 是否为共享文件下载

    TransportBar* bar;      // 进度控件
};

/* 云端文件信息 */
struct CloudFileInfo
{
    QString     userLogin;  // 所属用户
    QString     fileName;   // 文件名字
    QString     md5;        // 文件md5码
    QString     uploadTime; // 上传时间
    QString     url;        // URL
    QString     type;       // 文件类型
    qint64      size;       // 文件大小
    int         isShare;    // 是否共享 1共享 0不共享
    unsigned int  downloadCount;  // 下载次数
    QListWidgetItem* item;  // MyFileWidget 中对应该文件的 item
};

#endif // FILEINFO_H
