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
    JsonTool();

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
};

#endif // JSONTOOL_H
