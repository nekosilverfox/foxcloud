#ifndef JSONTOOL_H
#define JSONTOOL_H

#include "structs/foxcloudclientinfo.h"
#include "structs/userinfo.h"
#include "structs/webserverinfo.h"

class JsonTool
{
public:
    JsonTool();

    static bool overwriteFoxcloudClientInfoJsonFile(const FoxcloudClientInfo& clientInfo,
                                                    const QString& jsonPath);
    static bool overwriteFoxcloudClientInfoJsonFile(const QJsonObject jsonObjClientInfo,
                                                    const QString& jsonPath);

    static WebServerInfo getWebServerInfoFromJsonFile(const QString& jsonPath);
    static bool writeWebServerInfoToJsonFile(const WebServerInfo& webServerInfo, const QString& jsonPath);
};

#endif // JSONTOOL_H
