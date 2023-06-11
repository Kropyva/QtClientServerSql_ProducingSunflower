#ifndef CONNECT_H
#define CONNECT_H

#include <QJsonDocument>
#include <QStandardItemModel>

namespace connect {
    bool login(QByteArray& data);
    bool setModel(QStandardItemModel *model, QByteArray& data);
}
#endif // CONNECT_H
