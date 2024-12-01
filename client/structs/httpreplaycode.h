#ifndef HTTPREPLAYCODE_H
#define HTTPREPLAYCODE_H

/*
 * Server 返回的数据

*/
namespace HttpReplayCode
{
    /* 注册
     * 成功 {"code":"002"}
     * 用户已存在 {"code":"003"}
     * 失败 {"code":"004"}
     */
    namespace Reg
    {
        inline const char* EMPTY       =   "";
        inline const char* SUCCESS     = "002";
        inline const char* USER_EXISIT = "003";
        inline const char* FAIL        = "004";
    }

    /* 登录
     * 成功 {"code":"000"}
     * 失败 {"code":"001"}
     */
    namespace Login
    {
        inline const char* SUCCESS     = "000";
        inline const char* FAIL        = "001";
    }

}


#endif // HTTPREPLAYCODE_H
