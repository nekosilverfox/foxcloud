#include "clientinfoinstance.h"

#include <QDebug>

ClientInfoInstance* ClientInfoInstance::_instance = new ClientInfoInstance;


ClientInfoInstance::ClientInfoInstance() {}

/**
 * @brief ClientInfoInstance::getInstance 获取唯一实例
 * @return
 */
ClientInfoInstance *ClientInfoInstance::getInstance()
{
    return _instance;
}


void ClientInfoInstance::setClientInfo(const FoxcloudClientInfo &clientInfo, const QString &token)
{
    setClientInfo(clientInfo);
    setToken(token);
}

void ClientInfoInstance::setClientInfo(const FoxcloudClientInfo &clientInfo)
{
    if (clientInfo.userInfo.login.isEmpty()
        || clientInfo.webServerInfo.address.isEmpty()
        || 0 == clientInfo.webServerInfo.port)
    {
        qWarning() << "Login or Server info is empty";
    }
    _clientInfo = clientInfo;
}

void ClientInfoInstance::setToken(QString token)
{
    if (token.isEmpty())
    {
        qWarning() << "Token is empty";
    }

    _token = token;
}

FoxcloudClientInfo ClientInfoInstance::getClientInfo()
{
    return _clientInfo;
}

QString ClientInfoInstance::getLogin()
{
    return _clientInfo.userInfo.login;
}

QString ClientInfoInstance::getServerAddress()
{
    return _clientInfo.webServerInfo.address;
}

qint16 ClientInfoInstance::getServerPort()
{
    return _clientInfo.webServerInfo.port;
}

QString ClientInfoInstance::getToken()
{
    return _token;
}

