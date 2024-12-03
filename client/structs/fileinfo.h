#ifndef FILEINFO_H
#define FILEINFO_H

#include <QFile>
#include "transportbar.h"


/**
 * @brief 传输错误状态
 */
enum TransportStatus
{
    FILE_TOO_BIG,       // 文件过大
    ALREADY_IN_QUEUE,   // 文件已经在传输队列
    OPEN_FAILED,        // 打开文件失败
    GET_LAYOUT_FAILED   // 获取布局失败
};

/* 上传文件信息 */
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

#endif // FILEINFO_H
