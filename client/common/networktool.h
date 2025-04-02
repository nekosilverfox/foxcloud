#ifndef NETWORKTOOL_H
#define NETWORKTOOL_H

#include <QNetworkAccessManager>

#include "structs/fileinfo.h"

class NetworkTool
{
public:
    /* 用一次解析服务端发回来的数据 */
    static QNetworkAccessManager* getNetworkManager();

    static QString getReplayCode(const QByteArray& replayJsonData);
    static QString getReplayToken(const QByteArray& replayJsonData);
    static size_t  getReplayNumberFiles(const QByteArray& replayJsonData);
    static QList<CloudFileInfo*> getReplayCloudFilesList(const QByteArray& replayJsonData);

    static bool pingAddress(const QString& ip, const quint16 port = 80, int timeoutMs = 3000);

private:
    NetworkTool();
    ~NetworkTool();

private:
    // QNetworkAccessManager 本身是异步的，不会阻塞主线程
    static QNetworkAccessManager* _manager;  // 整个项目维护一个 QNetworkAccessManager 就足够了
};

#endif // NETWORKTOOL_H
