#ifndef OPERATIONS_H
#define OPERATIONS_H

#include <QJsonObject>
#include <QJsonDocument>
#include <QSqlDatabase>

namespace queries {
    QJsonDocument login(QJsonObject& received);
    QJsonDocument doQuery(QJsonObject& received, const QString& query);
    QJsonDocument getModel(QJsonObject& received, const QString& query = "SELECT * FROM view_data");
}

#endif // OPERATIONS_H
