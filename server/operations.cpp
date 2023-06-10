#include "operations.h"

#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QJsonArray>

static QSqlDatabase getDatabase(QJsonObject& jsonObject) {
    QSqlDatabase db(QSqlDatabase::addDatabase("QPSQL"));

    db.setHostName("localhost");
    db.setPort(5432);
    db.setDatabaseName("sunflower_production");
    db.setUserName(jsonObject["username"].toString());
    db.setPassword(jsonObject["password"].toString());

    return db;
}

static QJsonArray getJsonModel(QSqlDatabase& db, QString& queryText) {
    QSqlQueryModel *model(new QSqlQueryModel);
    model->setQuery(queryText, db);

    QJsonArray jsonArray {};
    QJsonObject jsonObject {};
    int rowCount(model->rowCount());
    int columnCount(model->columnCount());

    for (int row(0); row < rowCount; row++) {
        QJsonObject rowObject {};
        for (int column(0); column < columnCount; column++) {
            QModelIndex index(model->index(row, column));
            QString columnName(model->headerData(column, Qt::Horizontal).toString());
            QVariant value(model->data(index));
            rowObject[columnName] = QJsonValue::fromVariant(value);
        }
        jsonArray.append(rowObject);
    }

    delete model;
    return jsonArray;
}

QJsonObject queries::login(QJsonObject& received) {
    QSqlDatabase db { getDatabase(received) };
    QJsonObject response {};

    response["isOpen"] = db.open() ? true : false;

    db.close();
    return response;
}

QJsonObject queries::getModel(QJsonObject& received, QString queryText ) {
    QSqlDatabase db { getDatabase(received) };
    QJsonObject response {};

    if (db.open()) {
        response["model"] = getJsonModel(db, queryText);
    }

    db.close();
    return response;
}

static QJsonObject queries::doQuery(QJsonObject& received, QString& queryText) {
    QSqlDatabase db { getDatabase(received) };

    if (db.open()) {
        QSqlQuery *query(new QSqlQuery(db));
        query->prepare(queryText);
        query->exec();
        delete query;
    }

    db.close();
    return getModel(received);
}

