#include "encrypttool.h"

#include <QDebug>
#include <QCryptographicHash>
#include <QFile>

#include "des.h"

EncryptTool::EncryptTool() {}

/**
 * @brief EncryptTool::encryptPassword 加密小于 4K 的字符串，可以用于加密用户名和密码
 * @param str 要加密的字符串
 * @return 加密数据
 */
QByteArray EncryptTool::encryptString(const QString& str)
{
    qDebug() << "Start encrypt string" << str;

    unsigned char encData[1024] = {0};
    int encDataLen;

    // 加密
    int ret = DesDec((unsigned char*)str.toUtf8().data(), str.toUtf8().size(), encData, &encDataLen);
    if (0 != ret)
    {
        qWarning() << "Encrypt fail";
        return "";
    }

    // 再次通过 Base64 加密
    QByteArray result = QByteArray((char*)encData, encDataLen).toBase64();

    qDebug() << "Encrypt" << str << "to" << result;
    return result;
}

/**
 * @brief EncryptTool::getFileMD5 获取一个文件的 MD5 值
 * @param filePath 文件路径
 * @return
 */
QByteArray EncryptTool::getFileMD5(const QString& filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly))
    {
        qWarning() << "Can not open file" << filePath << file.errorString();
        return "";
    }

#if 0
    // Qt 官方说明中 addData 已经弃用
    const quint64 blockSize = 1024 * 1024 * 4;  // 每次计算 4MB 的数据，避免文件过大计算 MD5 值的时候慢
    while (!file.atEnd())
    {
        hash.addData(file.read(blockSize));
    }
#endif

    // 将文件数据加入哈希计算
    QCryptographicHash hash(QCryptographicHash::Md5);
    if (!hash.addData(&file))
    {
        qWarning() << "Failed to read data from file" << filePath;
        file.close();
        return "";
    }
    file.close();

    QByteArray result = hash.result();
    qInfo() << "Get file" << filePath << "Hash:" << result.toHex();
    return result;
}

/**
 * @brief EncryptTool::getStrMD5 获取一个字符串的 MD5 值
 * @param str
 * @return
 */
QByteArray EncryptTool::getStrMD5(const QString &str)
{
    return QCryptographicHash::hash(str.toUtf8(), QCryptographicHash::Md5);
}



