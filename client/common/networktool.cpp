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
        return HttpReplayCode::EMPTY;

    }

    if (doc.isNull() || doc.isEmpty() || !doc.isObject() || !doc.object().contains("code"))
    {
        qWarning() << "Can not get replay code, wrong json format: " << replayJsonData;
        return HttpReplayCode::EMPTY;
    }

    QString code = doc.object().value("code").toString();
    qInfo() << "Get replay code :" << code;

    return code;
}
