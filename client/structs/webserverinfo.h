#ifndef WEBSERVERINFO_H
#define WEBSERVERINFO_H

#include <QObject>

#define STR_VAL_ADDRESS     "address"
#define STR_VAL_PORT        "port"


struct WebServerInfo
{
    QString address = "";
    qint16  port = 0;
};

#endif // WEBSERVERINFO_H
