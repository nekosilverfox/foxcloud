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

/**
 * @brief JsonTool::getRegistrationJsonForServer 获取用于向Foxcloud服务器注册用户 POST 请求中所需的 JSON 数据
 * @param userInfo
 * @return 转换为 ByteArray 的二进制的 JSON 对象
 */
QByteArray JsonTool::getRegistrationJsonForServer(const UserInfo &userInfo)
{
    QJsonObject obj;
    obj.insert(JsonKeyForServer::Reg::STR_LOGIN,   userInfo.login);
    obj.insert(JsonKeyForServer::Reg::STR_NICKNAME, userInfo.nickname);
    obj.insert(JsonKeyForServer::Reg::STR_PASSWORD, userInfo.password);
    obj.insert(JsonKeyForServer::Reg::STR_PHONE,   userInfo.phone);
    obj.insert(JsonKeyForServer::Reg::STR_EMAIL,   userInfo.email);

    QJsonDocument doc = QJsonDocument(obj);
    return doc.toJson();
}

/**
 * @brief JsonTool::getLoginJsonForServer 获取用于向Foxcloud服务器登录用户 POST 请求中所需的 JSON 数据
 * @param userInfo
 * @return
 */
QByteArray JsonTool::getLoginJsonForServer(const UserInfo &userInfo)
{
    QJsonObject obj;
    obj.insert(JsonKeyForServer::Login::STR_LOGIN,      userInfo.login);
    obj.insert(JsonKeyForServer::Login::STR_PASSWORD,   userInfo.password);

    return QJsonDocument(obj).toJson();
}

/**
 * @brief JsonTool::getCheckMD5JsonForServer 用于获取向服务器检查文件 MD5 是否存在 JSON
 * @param login 用户 login
 * @param token 身份识别码
 * @param fileName 文件名
 * @param md5
 * @return 转换为 QByteArray 的二进制的 JSON 对象
 */
QByteArray JsonTool::getCheckMD5JsonForServer(const QString& login,    const QString& token,
                                              const QString& fileName, const QString& md5)
{
    /* Server 需要的 json 数据如下
    {
        user:xxxx,
        token:xxxx,
        md5:xxx,
        fileName: xxx
    }
    */
    QMap<QString, QVariant> map;
    map.insert(JsonKeyForServer::CheckMD5::STR_LOGIN, login);
    map.insert(JsonKeyForServer::CheckMD5::STR_TOKEN, token);
    map.insert(JsonKeyForServer::CheckMD5::STR_MD5,   md5);
    map.insert(JsonKeyForServer::CheckMD5::STR_FILENAME, fileName);

    qDebug() << "Get MD5 Json" << QJsonDocument::fromVariant(map).toJson();

    return QJsonDocument::fromVariant(map).toJson();
}

/**
 * @brief JsonTool::getUserNumberFilesJsonForServer 用于向服务器发送 Request 获取用户文件数量的 JSON
 * @param login 用户 login
 * @param token 身份识别码
 * @return
 */
QByteArray JsonTool::getUserNumberFilesJsonForServer(const QString &login, const QString &token)
{
    /*json数据如下
    {
        user:xxxx
        token: xxxx
    }
    */
    QJsonObject obj;
    obj.insert(JsonKeyForServer::UserFilesNumber::STR_LOGIN, login);
    obj.insert(JsonKeyForServer::UserFilesNumber::STR_TOKEN, token);

    return QJsonDocument(obj).toJson();
}

/**
 * @brief JsonTool::getUserFilesListJsonForServer 用于向服务器发送 Request 用于获取文件列表的 JSON
 * @param login 用户 login
 * @param token 身份识别码
 * @param start 起始位置
 * @param number 要获取的数量
 * @return
 */
QByteArray JsonTool::getUserFilesListJsonForServer(const QString &login, const QString &token,
                                                   const int start, const int number)
{
    /*{
        "user": "yoyo"
        "token": "xxx"
        "start": 0
        "count": 10
    }*/
    QJsonObject obj;
    obj.insert(JsonKeyForServer::UserFilesList::STR_LOGIN, login);
    obj.insert(JsonKeyForServer::UserFilesList::STR_TOKEN, token);
    obj.insert(JsonKeyForServer::UserFilesList::STR_START, start);
    obj.insert(JsonKeyForServer::UserFilesList::STR_NUMBER, number);

    return QJsonDocument(obj).toJson();
}

/**
 * @brief JsonTool::getCheckMD5JsonForServer 用于获取向服务器检查文件 MD5 是否存在 JSON
 * @param login 用户 login
 * @param token 身份识别码
 * @param fileName 文件名
 * @param md5
 * @return 转换为 QByteArray 的二进制的 JSON 对象
 */

/**
 * @brief JsonTool::getDelteFileJsonFromServer 用于获取向服务器删除文件的 JSON
 * @param login 用户 login
 * @param token 身份识别码
 * @param fileName 文件名
 * @param md5
 * @return
 */
QByteArray JsonTool::getDelteFileJsonFromServer(const QString &login, const QString &token, const QString &fileName, const QString &md5)
{
    /* Server 需要的 json 数据如下
    {
        user:xxxx,
        token:xxxx,
        fileName: xxx,
        md5:xxx
    }
    */
    QMap<QString, QVariant> map;
    map.insert(JsonKeyForServer::DeleteFile::STR_LOGIN, login);
    map.insert(JsonKeyForServer::DeleteFile::STR_TOKEN, token);
    map.insert(JsonKeyForServer::DeleteFile::STR_MD5,   md5);
    map.insert(JsonKeyForServer::DeleteFile::STR_FILENAME, fileName);

    qDebug() << "Delete file Json" << QJsonDocument::fromVariant(map).toJson();

    return QJsonDocument::fromVariant(map).toJson();
}







