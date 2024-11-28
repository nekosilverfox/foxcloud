#ifndef USERINFO_H
#define USERINFO_H

#include <QObject>

#define STR_VAL_LOGIN       "login"
#define STR_VAL_NICKNAME    "nickname"
#define STR_VAL_PASSWORD    "password"
#define STR_VAL_REMEMBER_PASSWORD "remember_password"
#define STR_VAL_PHONE       "phone"
#define STR_VAL_EMAIL       "email"


struct UserInfo
{
    QString     login       = "";
    QString     nickname    = "";
    QString     password    = "";
    QString     remember_password = "false";
    QString     phone       = "";
    QString     email       = "";
};

#endif // USERINFO_H
