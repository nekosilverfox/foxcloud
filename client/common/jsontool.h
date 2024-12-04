#ifndef JSONTOOL_H
#define JSONTOOL_H

#include "structs/foxcloudclientinfo.h"
#include "structs/userinfo.h"
#include "structs/webserverinfo.h"
#include "common/config.h"

#include <QJsonObject>

class JsonTool
{
public:
    /* 用于处理本地用户信息 */
    static QJsonObject userInfoToJsonObj(const UserInfo& userInfo);

    static FoxcloudClientInfo getFoxcloudClientInfo(const QString& jsonPath = PATH_FOXCLOUD_CLIENT_CONFIG);
    static bool overwriteFoxcloudClientInfo(const FoxcloudClientInfo& clientInfo,
                                            const QString& jsonPath = PATH_FOXCLOUD_CLIENT_CONFIG);
    static bool overwriteFoxcloudClientInfo(const QJsonObject jsonObjClientInfo,
                                            const QString& jsonPath = PATH_FOXCLOUD_CLIENT_CONFIG);

    static UserInfo getUserInfo(const QString& jsonPath = PATH_FOXCLOUD_CLIENT_CONFIG);
    static bool overwriteUserInfo(const UserInfo& userInfo,
                                  const QString& jsonPath = PATH_FOXCLOUD_CLIENT_CONFIG);

    static WebServerInfo getWebServerInfo(const QString& jsonPath = PATH_FOXCLOUD_CLIENT_CONFIG);
    static bool overwriteWebServerInfo(const WebServerInfo& webServerInfo,
                                       const QString& jsonPath = PATH_FOXCLOUD_CLIENT_CONFIG);

    /* 用于构造向服务器发送 Requests 中的 JSON */
    static QByteArray getRegistrationJsonForServer(const UserInfo& userInfo);
    static QByteArray getLoginJsonForServer(const UserInfo& userInfo);
    static QByteArray getCheckMD5JsonForServer(const QString& login, const QString& token,
                                                const QString& fileName, const QString& md5);
    static QByteArray getUserFilesNumberJsonForServer(const QString& login, const QString& token);
    static QByteArray getUserFilesListJsonForServer(const QString& login, const QString& token,
                                                    const int start, const int number);

private:
    JsonTool();
    ~JsonTool();
};

#endif // JSONTOOL_H
