#ifndef OPERATIONS_H
#define OPERATIONS_H

#include <QJsonObject>
#include <QSqlDatabase>

namespace queries {
    QJsonObject login(QJsonObject& received);
    QJsonObject doQuery(QJsonObject& received, QString& queryText);
    QJsonObject getModel(QJsonObject& received, QString queryText = "SELECT * FROM view_data");
}

#endif // OPERATIONS_H
