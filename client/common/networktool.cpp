#include "networktool.h"

#include <QListWidgetItem>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QIcon>

#include "structs/httpreplaycode.h"

/* 初始化变量 */
QNetworkAccessManager* NetworkTool::_manager = new QNetworkAccessManager;

NetworkTool::NetworkTool()
{}

QNetworkAccessManager* NetworkTool::getNetworkManager()
{
    return _manager;
}

/**
 * @brief NetworkTool::getReplayCode 得到服务器回复的状态码
 * @param replayJsonData
 * @return
 */
QString NetworkTool::getReplayCode(const QByteArray& replayJsonData)
{
    qInfo() << "Parses the reply from the server:" << replayJsonData;

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(replayJsonData, &error);
    if (error.error != QJsonParseError::NoError)
    {
        qCritical() << "Json format error:" << error.errorString();
        return HttpReplayCode::Reg::EMPTY;

    }

    if (doc.isNull() || doc.isEmpty() || !doc.isObject() || !doc.object().contains("code"))
    {
        qCritical() << "Can not get replay code, wrong json format: " << replayJsonData;
        return HttpReplayCode::Reg::EMPTY;
    }

    QString code = doc.object().value("code").toString();
    qInfo() << "Get replay code :" << code;

    return code;
}

/**
 * @brief NetworkTool::getReplayToken 解析并获取用户登录时服务器发回的用于身份验证的 Token
 * @param replayJsonData 服务器发回的 JSON 数据
 * @return token
 */
QString NetworkTool::getReplayToken(const QByteArray &replayJsonData)
{
    qInfo() << "Parses the reply from the server:" << replayJsonData;

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(replayJsonData, &error);
    if (error.error != QJsonParseError::NoError)
    {
        qCritical() << "Json format error:" << error.errorString();
        return HttpReplayCode::Reg::EMPTY;

    }

    if (doc.isNull() || doc.isEmpty() || !doc.isObject() || !doc.object().contains("token"))
    {
        qCritical() << "Can not get replay token, wrong json format: " << replayJsonData;
        return HttpReplayCode::Reg::EMPTY;
    }

    QString token = doc.object().value("token").toString();
    qDebug() << "Get replay token :" << token;

    return token;
}

/**
 * @brief NetworkTool::getReplayNumberFiles 解析并获取服务器发回的json，用户文件数量
 * @param replayJsonData 服务器发回的 JSON 数据
 * @return 文件数量
 */
size_t NetworkTool::getReplayNumberFiles(const QByteArray &replayJsonData)
{
    qInfo() << "Parses the reply from the server:" << replayJsonData;

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(replayJsonData, &error);
    if (error.error != QJsonParseError::NoError)
    {
        qCritical() << "Json format error:" << error.errorString();
        return 0;
    }

    if (doc.isNull() || doc.isEmpty() || !doc.isObject() || !doc.object().contains("num"))
    {
        qCritical() << "Can not get replay `num`, wrong json format: " << replayJsonData;
        return 0;
    }

    // 无法直接使用获得整型值：doc.object().value("num").toInt()
    // qDebug() << "Get replay num :" << doc.object().value("num").toString().toULongLong();

    return doc.object().value("num").toString().toULongLong();
}

/**
 * @brief NetworkTool::getReplayCloudFilesList 解析并构建云端☁️用户文件列表
 * @param replayJsonData 服务器发回的 JSON 数据
 * @return 包含有 CloudFileInfo 的列表
 */
QList<CloudFileInfo*> NetworkTool::getReplayCloudFilesList(const QByteArray& replayJsonData)
{
    qInfo() << "Parses the reply from the server:" << replayJsonData;

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(replayJsonData, &error);
    if (error.error != QJsonParseError::NoError)
    {
        qCritical() << "Json format error:" << error.errorString();
        return QList<CloudFileInfo*>();
    }

    if (doc.isNull() || doc.isEmpty() || !doc.isObject() || !doc.object().contains("files"))
    {
        qCritical() << "Can not get replay list `files`, wrong json format: " << replayJsonData;
        return QList<CloudFileInfo*>();
    }

    QJsonObject rootObj = doc.object();
    QJsonArray filesJsonArr = rootObj.value("files").toArray();
    size_t numFiles = filesJsonArr.size();
    qInfo() << "Number of user cloud files:" << numFiles;

    QList<CloudFileInfo*> resultList;
    for (auto i : filesJsonArr)
    {
        /*
            {
                "user": "xxxxxx",
                "md5": "xxxxxxxxxxxxxxxx",
                "time": "yyyy-mm-dd hh:mm:ss",
                "filename": "name.mp4",
                "share_status": 0,
                "pv": 0,
                "url": "http://192.168.1.1:80/group1/M00/00/00/xxxxxxxxxxxxxxxxx.mp4",
                "size": 2333333,
                "type": "mp4"
            }
        */
        QJsonObject curObj = i.toObject();
        CloudFileInfo* file = new CloudFileInfo;
        file->userLogin = curObj.value("user").toString();      // 所属用户
        file->fileName  = curObj.value("filename").toString();  // 文件名字
        file->md5       = curObj.value("md5").toString();       // 文件md5码
        file->uploadTime= curObj.value("time").toString();      // 上传时间
        file->url       = curObj.value("url").toString();       // URL
        file->type      = curObj.value("type").toString();      // 文件类型
        file->size      = curObj.value("size").toInt();         // 文件大小
        file->isShare   = curObj.value("share_status").toInt(); // 是否共享 1共享 0不共享
        file->downloadCount = curObj.value("pv").toInt();       // 下载次数
        file->item      = new QListWidgetItem(QIcon(":/img/foxcloud-logo-name.svg"), file->fileName);

        resultList.append(file);
        qDebug() << "Add user cloud file" << file->fileName << "to list";
    }

    return resultList;
}







