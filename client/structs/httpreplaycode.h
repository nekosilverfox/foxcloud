#ifndef HTTPREPLAYCODE_H
#define HTTPREPLAYCODE_H

/*
 * Server 返回的数据中 code 的对应值
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

    /* 通过 MD5 秒传文件：
     * 文件已存在：{"code":"005"}
     * 秒传成功：  {"code":"006"}
     * 秒传失败：  {"code":"007"}
     * token验证失败：{"code":"111"}
     */
    namespace CheckMD5
    {
        inline const char* FILE_EXIST   = "005";
        inline const char* SUCCESS      = "006";
        inline const char* FAIL         = "007";
        inline const char* TOKEN_ERROR  = "111";
    }

    /* 上传文件
     * 成功 {"code":"008"}
     * 失败 {"code":"009"}
     */
    namespace Upload
    {
        inline const char* SUCCESS     = "008";
        inline const char* FAIL        = "009";
    }

    namespace DeleteFile
    {
        inline const char* TOKEN_ERROR = "111";
        inline const char* SUCCESS     = "013";
        inline const char* FAIL        = "014";
    }

    namespace UserNumberCloudFiles
    {
        inline const char* SUCCESS     = "110";
        inline const char* FAIL        = "111";
    }

    /*
     * 获取文件列表
     */
    namespace GetUserFilesList
    {
        inline const char* TOKEN_ERROR = "111";
        inline const char* FAIL        = "015";
    }
}


#endif // HTTPREPLAYCODE_H
