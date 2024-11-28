#include "jsontool.h"

#include <QFile>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>

JsonTool::JsonTool() {}

/**
 * @brief JsonTool::overwriteFoxcloudClientInfoJsonFile 复写存储 FoxcloudClientInfo 的文件，如果不存在则会新建、如果存在则复写
 * @param clientInfo
 * @param jsonPath
 * @return
 */
bool JsonTool::overwriteFoxcloudClientInfoJsonFile(const FoxcloudClientInfo& clientInfo, const QString& jsonPath)
{
    QFile file(jsonPath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        qWarning() << "Failed to open file:" << file.errorString();
        return false;
    }

    QJsonObject objUserInfo;
    objUserInfo.insert(STR_VAL_LOGIN,    QJsonValue(clientInfo.userInfo.login));
    objUserInfo.insert(STR_VAL_NICKNAME, QJsonValue(clientInfo.userInfo.nickname));
    objUserInfo.insert(STR_VAL_PASSWORD, QJsonValue(clientInfo.userInfo.password));
    objUserInfo.insert(STR_VAL_REMEMBER_PASSWORD, QJsonValue(clientInfo.userInfo.remember_password));
    objUserInfo.insert(STR_VAL_PHONE,    QJsonValue(clientInfo.userInfo.phone));
    objUserInfo.insert(STR_VAL_EMAIL,    QJsonValue(clientInfo.userInfo.email));

    QJsonObject objWebServerInfo;
    objWebServerInfo.insert(STR_VAL_ADDRESS, QJsonValue(clientInfo.webServerInfo.address));
    objWebServerInfo.insert(STR_VAL_PORT,    QJsonValue(clientInfo.webServerInfo.port));

    QJsonObject objRoot;
    objRoot.insert(STR_OBJ_USERINFO,        objUserInfo);
    objRoot.insert(STR_OBJ_WEBSERVERINFO,   objWebServerInfo);

    file.write(QJsonDocument(objRoot).toJson());
    file.close();

    qDebug() << "Overwrite FoxcloudClientInfo object to " << jsonPath;
    return true;
}

/**
 * @brief JsonTool::overwriteFoxcloudClientInfoJsonFile 复写存储 FoxcloudClientInfo 的文件，如果不存在则会新建、如果存在则复写
 * @param jsonObjClientInfo
 * @param jsonPath
 * @return
 */
bool JsonTool::overwriteFoxcloudClientInfoJsonFile(const QJsonObject jsonObjClientInfo, const QString &jsonPath)
{
    QFile file(jsonPath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        qWarning() << "Failed to open file:" << file.errorString();
        return false;
    }

    QJsonDocument doc(jsonObjClientInfo);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qWarning() <<"Can not open file";
        return false;
    }
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();

    qDebug() << "Overwrite QJsonObject of client info to " << jsonPath;
    return true;
}

/**
 * @brief JsonTool::getWebServerInfoFromJsonFile 从 JSON 文件中获取 WebServerInfo
 * @param jsonPath JSON 文件的路径
 * @return WebServerInfo
 */
WebServerInfo JsonTool::getWebServerInfoFromJsonFile(const QString& jsonPath)
{
    QFile file(jsonPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning() << "Can not open file" << jsonPath;
        return WebServerInfo();
    }

    // 读取文件内容
    QByteArray fileData = file.readAll();
    file.close();

    // 解析 JSON
    QJsonDocument doc = QJsonDocument::fromJson(fileData);
    if (doc.isNull() || !doc.isObject())
    {
        qWarning() << "Invalid JSON file" << jsonPath;
        return WebServerInfo();
    }

    // 获取根对象
    QJsonObject objRoot = doc.object();

    // 这里我们获取 web_server
    if (!objRoot.contains(STR_OBJ_WEBSERVERINFO) || !objRoot[STR_OBJ_WEBSERVERINFO].isObject())
    {
        qWarning() << "JSON does not contain 'web_server' object" << jsonPath;
        return WebServerInfo();
    }
    QJsonObject objWebServer = objRoot[STR_OBJ_WEBSERVERINFO].toObject();
    WebServerInfo webServerInfo;
    webServerInfo.address = objWebServer[STR_VAL_ADDRESS].toString();
    webServerInfo.port = (qint16)objWebServer[STR_VAL_PORT].toInt();

    qDebug() << "Get " << STR_OBJ_WEBSERVERINFO << ": [" << STR_VAL_ADDRESS << ": " << webServerInfo.address
             << "], [" << STR_VAL_PORT << ": " << webServerInfo.port << "] from JSON file " << jsonPath;
    return webServerInfo;
}

/**
 * @brief JsonTool::overwriteWebServerInfoToFile 将 WebServerInfo 复写到指定JSON文件 中，如果文件没有则会新建并写入
 * @param webServerInfo WebServerInfo 对象
 * @param jsonPath JSON 路径
 * @return 是否成功
 */
bool JsonTool::overwriteWebServerInfoToFile(const WebServerInfo& webServerInfo, const QString& jsonPath)
{
    // 如果文件不存在，直接创建一个带有WebServerInfo默认值的 JSON 新文件
    if (!QFile::exists(jsonPath))
    {
        FoxcloudClientInfo clientInfo;
        clientInfo.webServerInfo = webServerInfo;
        overwriteFoxcloudClientInfoJsonFile(clientInfo, jsonPath);
        return true;
    }

    QFile file(jsonPath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qWarning() << "Can not open file " << jsonPath;
        return false;
    }

    // 读取文件内容
    QByteArray fileData = file.readAll();
    file.close();

    // 解析 JSON
    QJsonDocument doc = QJsonDocument::fromJson(fileData);
    if (doc.isNull() || !doc.isObject())
    {
        qWarning() << "Invalid JSON file " << jsonPath;
        return false;
    }

    // 获取根对象
    QJsonObject objRoot = doc.object();

    // 这里我们修改 WebServerInfo
    if (!objRoot.contains(STR_OBJ_WEBSERVERINFO) || !objRoot[STR_OBJ_WEBSERVERINFO].isObject())
    {
        qWarning() << "JSON does not contain 'web_server' object";
        return false;
    }

    // 更新 web_server 的值
    QJsonObject objWebServer = objRoot[STR_OBJ_WEBSERVERINFO].toObject();
    objWebServer[STR_VAL_ADDRESS] = webServerInfo.address;
    objWebServer[STR_VAL_PORT] = webServerInfo.port;

    // 更新根对象
    objRoot[STR_OBJ_WEBSERVERINFO] = objWebServer;

    // 写回 JSON 文件
    qDebug() << "Overwrite " << STR_OBJ_WEBSERVERINFO << " to [" << STR_VAL_ADDRESS << ": " << webServerInfo.address
             << "], [" << STR_VAL_PORT << ": " << webServerInfo.port << "] to JSON file " << jsonPath;

    return overwriteFoxcloudClientInfoJsonFile(objRoot, jsonPath);
}

