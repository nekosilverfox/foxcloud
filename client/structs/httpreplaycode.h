#ifndef HTTPREPLAYCODE_H
#define HTTPREPLAYCODE_H

/*
 * 读取返回的数据
 * 成功 {"code":"002"}
 * 用户已存在 {"code":"003"}
 * 失败 {"code":"004"}
*/
namespace HttpReplayCode
{
inline const char* EMPTY       =   "";
inline const char* SUCCESS     = "002";
inline const char* USER_EXISIT = "003";
inline const char* FAIL        = "004";
}


#endif // HTTPREPLAYCODE_H
