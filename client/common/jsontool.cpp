#include "jsontool.h"

#include <QFile>
#include <QDebug>
#include <QJsonDocument>


/**
 * @brief JsonTool::userInfoToJsonObj 将给定的 UserInfo 转换为 QJsonObject
 * @param userInfo
 * @return
 */
QJsonObject JsonTool::userInfoToJsonObj(const UserInfo &userInfo)
{
    QJsonObject objUserInfo;
    objUserInfo.insert(STR_VAL_LOGIN,    QJsonValue(userInfo.login));
    objUserInfo.insert(STR_VAL_NICKNAME, QJsonValue(userInfo.nickname));
    objUserInfo.insert(STR_VAL_PASSWORD, QJsonValue(userInfo.password));
    objUserInfo.insert(STR_VAL_REMEMBER_PASSWORD, QJsonValue(userInfo.remember_password));
    objUserInfo.insert(STR_VAL_PHONE,    QJsonValue(userInfo.phone));
    objUserInfo.insert(STR_VAL_EMAIL,    QJsonValue(userInfo.email));

    return objUserInfo;
}

/**
 * @brief JsonTool::getFoxcloudClientInfo 从 JSON 文件中获取 FoxcloudClientInfo 对象
 * @param jsonPath JSON 文件路径
 * @return 文件不存在返回一个空对象
 */
FoxcloudClientInfo JsonTool::getFoxcloudClientInfo(const QString &jsonPath)
{
    // 如果文件不存在
    QFile file(jsonPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning() << "Can not open file " << jsonPath;
        return FoxcloudClientInfo();
    }

    // 读取文件内容
    QByteArray fileData = file.readAll();
    file.close();

    // 解析 JSON
    QJsonDocument doc = QJsonDocument::fromJson(fileData);
    if (doc.isNull() || !doc.isObject())
    {
        qWarning() << "Invalid JSON file " << jsonPath;
        return FoxcloudClientInfo();
    }

    // 获取根对象
    FoxcloudClientInfo clentInfo;
    QJsonObject objRoot = doc.object();

    // 读取 STR_OBJ_USERINFO
    if (!objRoot.contains(STR_OBJ_USERINFO) || !objRoot[STR_OBJ_USERINFO].isObject())
    {
        qWarning() << "JSON does not contain '" << STR_OBJ_USERINFO << "' object";
    }
    else
    {
        QJsonObject objUserInfo     = objRoot[STR_OBJ_USERINFO].toObject();
        clentInfo.userInfo.login    =   objUserInfo[STR_VAL_LOGIN].toString();
        clentInfo.userInfo.nickname =   objUserInfo[STR_VAL_NICKNAME].toString();
        clentInfo.userInfo.password =   objUserInfo[STR_VAL_PASSWORD].toString();
        clentInfo.userInfo.remember_password  = objUserInfo[STR_VAL_REMEMBER_PASSWORD].toString();
        clentInfo.userInfo.email    =   objUserInfo[STR_VAL_EMAIL].toString();
        clentInfo.userInfo.phone    =   objUserInfo[STR_VAL_PHONE].toString();
    }

    // 读取 STR_OBJ_WEBSERVERINFO
    if (!objRoot.contains(STR_OBJ_WEBSERVERINFO) || !objRoot[STR_OBJ_WEBSERVERINFO].isObject())
    {
        qWarning() << "JSON does not contain '" << STR_OBJ_WEBSERVERINFO << "' object";
    }
    else
    {
        QJsonObject objWebServer = objRoot[STR_OBJ_WEBSERVERINFO].toObject();
        clentInfo.webServerInfo.address   = objWebServer[STR_VAL_ADDRESS].toString();
        clentInfo.webServerInfo.port      = (qint16)objWebServer[STR_VAL_PORT].toInt();
    }

    file.close();
    return clentInfo;
}

/**
 * @brief JsonTool::overwriteFoxcloudClientInfo 复写存储 FoxcloudClientInfo 的文件，如果不存在则会新建、如果存在则复写
 * @param clientInfo
 * @param jsonPath
 * @return
 */
bool JsonTool::overwriteFoxcloudClientInfo(const FoxcloudClientInfo& clientInfo, const QString& jsonPath)
{
    QFile file(jsonPath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        qWarning() << "Failed to open file:" << file.errorString();
        return false;
    }

    QJsonObject objUserInfo = userInfoToJsonObj(clientInfo.userInfo);

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
 * @brief JsonTool::overwriteFoxcloudClientInfo 复写存储 FoxcloudClientInfo 的文件，如果不存在则会新建、如果存在则复写
 * @param jsonObjClientInfo
 * @param jsonPath
 * @return
 */
bool JsonTool::overwriteFoxcloudClientInfo(const QJsonObject jsonObjClientInfo, const QString &jsonPath)
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
 * @brief JsonTool::getUserInfo 从 JSON 文件中获取 UserInfo 对象
 * @param jsonPath
 * @return UserInfo 对象，如果文件不存在或者已将将返回空 UserInfo 对象
 */
UserInfo JsonTool::getUserInfo(const QString &jsonPath)
{
    FoxcloudClientInfo clientInfo = getFoxcloudClientInfo(jsonPath);
    return clientInfo.userInfo;
}

/**
 * @brief JsonTool::overwriteUserInfo 将 UserInfo 复写到指定JSON文件 中，如果文件没有则会新建并写入
 * @param userInfo
 * @param jsonPath
 * @return
 */
bool JsonTool::overwriteUserInfo(const UserInfo &userInfo, const QString &jsonPath)
{
    qDebug() << "Start overwrite UserInfo to file";

    FoxcloudClientInfo clientInfo = getFoxcloudClientInfo(jsonPath);

    // 更新 userInfo 的值
    clientInfo.userInfo = userInfo;

    // 写回 JSON 文件
    qDebug() << "Overwrite " << STR_OBJ_USERINFO << " to "
             << "[" << STR_VAL_LOGIN    << ": " << userInfo.login       << "],"
             << "[" << STR_VAL_NICKNAME << ": " << userInfo.nickname    << "]"
             << "[" << STR_VAL_PASSWORD << ": " << userInfo.password    << "]"
             << "[" << STR_VAL_REMEMBER_PASSWORD    << ": " << userInfo.remember_password    << "]"
             << "[" << STR_VAL_EMAIL    << ": " << userInfo.email       << "]"
             << "[" << STR_VAL_PHONE    << ": " << userInfo.phone       << "]"
             << "to JSON file " << jsonPath;

    return overwriteFoxcloudClientInfo(clientInfo, jsonPath);
}

/**
 * @brief JsonTool::getWebServerInfo 从 JSON 文件中获取 WebServerInfo
 * @param jsonPath JSON 文件的路径
 * @return WebServerInfo
 */
WebServerInfo JsonTool::getWebServerInfo(const QString& jsonPath)
{
    FoxcloudClientInfo clientInfo = getFoxcloudClientInfo(jsonPath);

    qDebug() << "Get " << STR_OBJ_WEBSERVERINFO << ": [" << STR_VAL_ADDRESS << ": " << clientInfo.webServerInfo.address
             << "], [" << STR_VAL_PORT << ": " << clientInfo.webServerInfo.port << "] from JSON file " << jsonPath;
    return clientInfo.webServerInfo;
}

/**
 * @brief JsonTool::overwriteWebServerInfo 将 WebServerInfo 复写到指定JSON文件 中，如果文件没有则会新建并写入
 * @param webServerInfo WebServerInfo 对象
 * @param jsonPath JSON 路径
 * @return 是否成功
 */
bool JsonTool::overwriteWebServerInfo(const WebServerInfo& webServerInfo, const QString& jsonPath)
{
    qDebug() << "Start overwrite WebServerInfo to file";
    FoxcloudClientInfo clientInfo = getFoxcloudClientInfo(jsonPath);

    // 更新 web_server 的值
    clientInfo.webServerInfo = webServerInfo;

    // 写回 JSON 文件
    qDebug() << "Overwrite " << STR_OBJ_WEBSERVERINFO << " to [" << STR_VAL_ADDRESS << ": " << webServerInfo.address
             << "], [" << STR_VAL_PORT << ": " << webServerInfo.port << "] to JSON file " << jsonPath;

    return overwriteFoxcloudClientInfo(clientInfo, jsonPath);
}

