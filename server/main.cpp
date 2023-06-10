#include "operations.h"

#include <QCoreApplication>
#include <QTcpServer>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QTcpServer server;

    QObject::connect(&server, &QTcpServer::newConnection, &server, [&](){
        QTcpSocket *socket(std::move(server.nextPendingConnection()));

        if (!socket->waitForReadyRead(5000)) {
            qDebug() << ">> Error: " << socket->errorString();
            return;
        }

        qDebug() << ">> Client connected";

        QByteArray data(socket->readAll());
        QJsonDocument jsonDocument(QJsonDocument::fromJson(data));
        QJsonObject jsonObject(jsonDocument.object());

        qDebug() << ">> Received object:\n" << jsonObject;

        QJsonObject responseObject {};

        if (jsonObject["command"] == "login") {
            responseObject = queries::login(jsonObject);
        } else if(jsonObject["command"] == "table") {
            responseObject = queries::getModel(jsonObject);
        } else if(jsonObject["command"] == "insert") {
            QString query {
                "INSERT INTO sunflower VALUES ("
                + jsonObject["area"].toString() + " * 1000000, "
                + jsonObject["production"].toString() + " * 1000000, MAKE_DATE("
                + jsonObject["year"].toString() + ", 1, 1));"
            };

            responseObject = queries::doQuery(jsonObject, query);
        } else if(jsonObject["command"] == "update") {
            QString query {
                "UPDATE sunflower SET area = "
                + jsonObject["area"].toString()
                + " * 1000000, production ="
                + jsonObject["production"].toString()
                + " * 1000000 WHERE year = MAKE_DATE("
                + jsonObject["year"].toString()
                + ", 1, 1)"
            };

            responseObject = queries::doQuery(jsonObject, query);
        } else if(jsonObject["command"] == "delete") {
            QString query {
                "DELETE FROM sunflower WHERE year = MAKE_DATE("
                + jsonObject["year"].toString() + ", 1, 1);"
            };

            responseObject = queries::doQuery(jsonObject, query);
        }

        QJsonDocument responseDocument(responseObject);
        QByteArray responseUtf8(responseDocument.toJson(QJsonDocument::Compact));

        socket->write(responseUtf8);
        socket->flush();

        qDebug() << ">> Sent object:\n" << responseObject;

        socket->deleteLater();
        socket->disconnectFromHost();
        socket->close();
        delete socket;

        qDebug() << ">> Client disconnected";
    });

    if (!server.listen(QHostAddress::LocalHost, 9090)) {
        qDebug() << ">> Server could not start!";
    } else {
        qDebug() << ">> Server started and listening on port 9090!";
    }

    return a.exec();
}
