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
    static bool overwriteFoxcloudClientInfoJsonFile(const FoxcloudClientInfo& clientInfo,
                                                    const QString& jsonPath = PATH_FOXCLOUD_CLIENT_CONFIG);
    static bool overwriteFoxcloudClientInfoJsonFile(const QJsonObject jsonObjClientInfo,
                                                    const QString& jsonPath = PATH_FOXCLOUD_CLIENT_CONFIG);

    static UserInfo getUserInfo(const QString& jsonPath = PATH_FOXCLOUD_CLIENT_CONFIG);
    static bool overwriteUserInfoToFile(const UserInfo& userInfo,
                                        const QString& jsonPath = PATH_FOXCLOUD_CLIENT_CONFIG);

    static WebServerInfo getWebServerInfoFromJsonFile(const QString& jsonPath = PATH_FOXCLOUD_CLIENT_CONFIG);
    static bool overwriteWebServerInfoToFile(const WebServerInfo& webServerInfo,
                                             const QString& jsonPath = PATH_FOXCLOUD_CLIENT_CONFIG);
};

#endif // JSONTOOL_H
