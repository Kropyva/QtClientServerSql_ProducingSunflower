#ifndef CONNECT_H
#define CONNECT_H

#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QStandardItemModel>

namespace connect {
    bool login(QByteArray& data);
    void query(QStandardItemModel *model, QByteArray& data);
}
#endif // CONNECT_H
