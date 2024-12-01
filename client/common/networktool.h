#ifndef NETWORKTOOL_H
#define NETWORKTOOL_H

#include <QNetworkAccessManager>


class NetworkTool
{
public:
    NetworkTool();

    static QNetworkAccessManager* getNetworkManager();
    static QString getReplayCode(const QByteArray& replayJsonData);

private:
    static QNetworkAccessManager* _manager;  // 整个项目维护一个 QNetworkAccessManager 就足够了
};

#endif // NETWORKTOOL_H
