#include "qaesencryption.h"
#include "crypto.h"

QByteArray key("0123456789abcdef");
QByteArray iv("fedcba9876543210");

static QByteArray trim(QByteArray data) {
    if (data.isEmpty()) {
        return data;
    }

    while(data.back() == '\0') {
        data.resize(data.size() - 1);
    }

    return data;
}

QByteArray crypto::encrypt(const QByteArray &message) {
    return QAESEncryption::Crypt(QAESEncryption::AES_128, QAESEncryption::OFB, message, key, iv, QAESEncryption::ZERO);
}

QByteArray crypto::decrypt(const QByteArray &encrypted) {
    return trim(QAESEncryption::Decrypt(QAESEncryption::AES_128, QAESEncryption::OFB, encrypted, key, iv, QAESEncryption::ZERO));
}


