#include "connect.h"

static void write(QTcpSocket& socket, QByteArray& data) {
    socket.connectToHost(QHostAddress::LocalHost, 9090);

    if (!socket.waitForConnected(5000)) {
        socket.close();
        return;
    }

    socket.write(data);
    socket.flush();
}

static QJsonObject read(QTcpSocket& socket) {
    QJsonObject response {};

    if (!socket.waitForReadyRead(5000)) {
        return response;
    }

    QByteArray data(socket.readAll());
    QJsonDocument jsonDocument(QJsonDocument::fromJson(data));
    response = jsonDocument.object();

    return response;
}

static QJsonObject conn(QByteArray& data) {
    QTcpSocket socket {};
    QJsonObject response {};

    write(socket, data);

    if (socket.isOpen()) {
        response = read(socket);
        socket.close();
    }

    return response;
}

bool connect::login(QByteArray& data) {
    return conn(data)["isOpen"].toBool() ? true : false;
}

void connect::query(QStandardItemModel *model, QByteArray& data) {
    if (QJsonDocument::fromJson(data).isNull()) {
        return;
    }

    QJsonArray jsonArray(conn(data)["model"].toArray());
    if (jsonArray.isEmpty()) {
        return;
    }

    QStringList keys(jsonArray[0].toObject().keys());
    int columnCount(keys.count());
    int rowCount(jsonArray.size());

    model->setHorizontalHeaderLabels(keys);
    model->setColumnCount(columnCount);
    model->setRowCount(rowCount);

    for (int row(0); row < rowCount; row++) {
        QJsonObject jsonObject(jsonArray[row].toObject());
        for(int column(0); column < columnCount; column++) {
            const QString& key(keys.at(column));
            const QVariant& value(jsonObject.value(key).toVariant());
            QModelIndex index(model->index(row, column));
            model->setData(index, value);
        }
    }
}
