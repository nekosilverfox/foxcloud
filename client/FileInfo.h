#ifndef FILEINFO_H
#define FILEINFO_H

#include <QFile>
#include "transportbar.h"

/* 上传文件信息 */
struct UploadFileInfo
{
    QFile*  file;       // 文件指针
    QString md5;        // 文件md5码
    QString name;       // 文件名字
    QString path;       // 文件路径
    qint64  size;       // 文件大小
    bool    isUploading;// 是否已经在上传
    TransportBar* bar;  // 上传进度控件
};

#endif // FILEINFO_H
