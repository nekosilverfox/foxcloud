#ifndef FOXCLOUDCLIENTINFO_H
#define FOXCLOUDCLIENTINFO_H

#include "structs/userinfo.h"
#include "structs/webserverinfo.h"

#define STR_OBJ_USERINFO        "user"
#define STR_OBJ_WEBSERVERINFO   "web_server"

struct FoxcloudClientInfo
{
    UserInfo        userInfo;
    WebServerInfo   webServerInfo;
};

#endif // FOXCLOUDCLIENTINFO_H
