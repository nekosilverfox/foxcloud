#ifndef CLIENTINFOINSTANCE_H
#define CLIENTINFOINSTANCE_H

#include <QString>

#include "structs/foxcloudclientinfo.h"

/**
 * @brief 单例类，用于存储当前登录用户的配置信息（用户名、服务器 ip、端口等）
 */
class ClientInfoInstance
{
public:
    static ClientInfoInstance* getInstance();

    void setClientInfo(const FoxcloudClientInfo& clientInfo, const QString& token);
    void setClientInfo(const FoxcloudClientInfo& clientInfo);
    void setToken(QString token);

    FoxcloudClientInfo getClientInfo();
    QString getLogin();
    QString getServerAddress();
    qint16  getServerPort();
    QString getToken();

private:
    ClientInfoInstance();
    ~ClientInfoInstance();

    // 把复制构造函数和=操作符也设为私有, 防止被复制
    ClientInfoInstance(const ClientInfoInstance&);
    ClientInfoInstance& operator=(const ClientInfoInstance&);


private:
    static ClientInfoInstance* _instance;

    FoxcloudClientInfo _clientInfo;
    QString _token;
};

#endif // CLIENTINFOINSTANCE_H
