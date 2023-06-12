#include "connect.h"
#include "crypto.h"

#include <QTcpSocket>
#include <QJsonArray>
#include <QJsonObject>

static void write(QTcpSocket& socket, QByteArray& data) {
    socket.connectToHost(QHostAddress::LocalHost, 9090);

    if (!socket.waitForConnected(5000)) {
        socket.close();
        return;
    }

    socket.write(crypto::encrypt(data));
    socket.flush();
}

static QJsonDocument read(QTcpSocket& socket) {
    if (!socket.waitForReadyRead(5000)) {
        return QJsonDocument();
    }

    QByteArray data(crypto::decrypt(socket.readAll()));
    return QJsonDocument::fromJson(data);
}

static QJsonDocument conn(QByteArray& data) {
    QTcpSocket socket {};
    QJsonDocument response {};

    write(socket, data);

    if (socket.isOpen()) {
        response = read(socket);
        socket.close();
    }

    return response;
}

bool connect::login(QByteArray& data) {
    return conn(data).object()["login"].toBool() ? true : false;
}

bool connect::setModel(QStandardItemModel *model, QByteArray& data) {
    QJsonDocument document(conn(data));
    if (document.isNull()) {
        return false;
    }

    QJsonArray array(document.array());
    if (array.isEmpty()) {
        return false;
    }

    QStringList keys(array[0].toObject().keys());
    int columnCount(keys.count());
    int rowCount(array.size());

    model->setHorizontalHeaderLabels(keys);
    model->setColumnCount(columnCount);
    model->setRowCount(rowCount);

    for (int row(0); row < rowCount; row++) {
        QJsonObject jsonObject(array[row].toObject());
        for(int column(0); column < columnCount; column++) {
            const QString& key(keys.at(column));
            const QVariant& value(jsonObject.value(key).toVariant());
            QModelIndex index(model->index(row, column));
            model->setData(index, value);
        }
    }

    return true;
}
