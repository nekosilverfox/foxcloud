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

/**
 * @brief ClientInfoInstance::destory 销毁当前实例
 *
 * 单例对象的销毁操作应该通过静态成员变量管理，而不应该 delete this->_instance 销毁，因为 _instance 是静态成员，与具体的 this 实例无关
 */
void ClientInfoInstance::destory()
{
    if (nullptr == ClientInfoInstance::_instance)
    {
        return;
    }

    delete ClientInfoInstance::_instance;
    ClientInfoInstance::_instance = nullptr;

    qInfo() << "ClientInfoInstance instance was destoryed";
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

QString ClientInfoInstance::getLogin()
{
    return _clientInfo.userInfo.login;
}

QString ClientInfoInstance::getServerAddress()
{
    return _clientInfo.webServerInfo.address;
}

qint16 ClientInfoInstance::getServerIp()
{
    return _clientInfo.webServerInfo.port;
}

QString ClientInfoInstance::getToken()
{
    return _token;
}

