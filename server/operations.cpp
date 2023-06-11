#include "operations.h"

#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QJsonArray>

static QSqlDatabase getDatabase(QJsonObject& obj) {
    QSqlDatabase db(QSqlDatabase::addDatabase("QPSQL"));

    db.setHostName("localhost");
    db.setPort(5432);
    db.setDatabaseName("sunflower_production");
    db.setUserName(obj["username"].toString());
    db.setPassword(obj["password"].toString());

    return db;
}

static QJsonArray getJsonModel(QSqlDatabase& db, const QString& query) {
    QSqlQueryModel *model(new QSqlQueryModel);
    model->setQuery(query, db);

    QJsonArray array {};
    QJsonObject obj {};
    int rowCount(model->rowCount());
    int columnCount(model->columnCount());

    for (int row(0); row < rowCount; row++) {
        QJsonObject rowObject {};
        for (int column(0); column < columnCount; column++) {
            QModelIndex index(model->index(row, column));
            QString key(model->headerData(column, Qt::Horizontal).toString());
            QVariant value(model->data(index));
            rowObject[key] = QJsonValue::fromVariant(value);
        }
        array.append(rowObject);
    }

    delete model;
    return array;
}

QJsonDocument queries::login(QJsonObject& received) {
    QSqlDatabase db { getDatabase(received) };
    QJsonDocument response {};
    QJsonObject obj {};

    obj["login"] = db.open() ? true : false;
    db.close();

    response.setObject(obj);
    return response;
}

QJsonDocument queries::getModel(QJsonObject& received, const QString& query ) {
    QSqlDatabase db { getDatabase(received) };
    QJsonDocument response {};

    if (db.open()) {
        response.setArray(getJsonModel(db, query));
    }

    db.close();
    return response;
}

QJsonDocument queries::doQuery(QJsonObject& received, const QString& query) {
    QSqlDatabase db { getDatabase(received) };

    if (db.open()) {
        QSqlQuery *sqlQuery(new QSqlQuery(db));
        sqlQuery->prepare(query);
        sqlQuery->exec();
        delete sqlQuery;
    }

    db.close();
    return getModel(received);
}

