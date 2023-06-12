#ifndef CRYPTO_H
#define CRYPTO_H

#include <QByteArray>

namespace crypto {
    QByteArray encrypt(const QByteArray &message);
    QByteArray decrypt(const QByteArray &encrypted);
}

#endif // CRYPTO_H
