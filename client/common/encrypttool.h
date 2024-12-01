#ifndef ENCRYPTTOOL_H
#define ENCRYPTTOOL_H

#include <QString>

class EncryptTool
{
public:
    EncryptTool();

    static QByteArray encryptString(const QString& str);

    static QByteArray getFileMD5(const QString& filePath);
    static QByteArray getStrMD5(const QString& str);

};

#endif // ENCRYPTTOOL_H
