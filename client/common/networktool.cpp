#include "networktool.h"

#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>

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
    qInfo() << "Get replay from server: " << replayJsonData;

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(replayJsonData, &error);
    if (error.error != QJsonParseError::NoError)
    {
        qCritical() << "Json format error:" << error.errorString();
        return HttpReplayCode::Reg::EMPTY;

    }

    if (doc.isNull() || doc.isEmpty() || !doc.isObject() || !doc.object().contains("code"))
    {
        qWarning() << "Can not get replay code, wrong json format: " << replayJsonData;
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
    qInfo() << "Get replay from server: " << replayJsonData;

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(replayJsonData, &error);
    if (error.error != QJsonParseError::NoError)
    {
        qCritical() << "Json format error:" << error.errorString();
        return HttpReplayCode::Reg::EMPTY;

    }

    if (doc.isNull() || doc.isEmpty() || !doc.isObject() || !doc.object().contains("token"))
    {
        qWarning() << "Can not get replay token, wrong json format: " << replayJsonData;
        return HttpReplayCode::Reg::EMPTY;
    }

    QString token = doc.object().value("token").toString();
    qDebug() << "Get replay token :" << token;

    return token;
}
