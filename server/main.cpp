#include "operations.h"
#include "crypto.h"

#include <QCoreApplication>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QTcpServer server;

    QObject::connect(&server, &QTcpServer::newConnection, &server, [&server](){
        QTcpSocket *socket(server.nextPendingConnection());

        if (!socket->waitForReadyRead(5000)) {
            qDebug() << ">> Error: " << socket->errorString();
            return;
        }

        qDebug() << ">> Client connected";

        QByteArray data(crypto::decrypt(socket->readAll()));
        QJsonDocument document(QJsonDocument::fromJson(data));
        QJsonObject obj(document.object());

        qDebug() << ">> Received object:\n" << document;

        if (obj["command"] == "login") {
            document = queries::login(obj);

        } else if(obj["command"] == "insert") {
            QString query {
                "INSERT INTO sunflower VALUES ("
                + obj["area"].toString() + " * 1000000, "
                + obj["production"].toString() + " * 1000000, MAKE_DATE("
                + obj["year"].toString() + ", 1, 1));"
            };
            document = queries::doQuery(obj, query);

        } else if(obj["command"] == "update") {
            QString query {
                "UPDATE sunflower SET area = "
                + obj["area"].toString()
                + " * 1000000, production ="
                + obj["production"].toString()
                + " * 1000000 WHERE year = MAKE_DATE("
                + obj["year"].toString()
                + ", 1, 1)"
            };
            document = queries::doQuery(obj, query);

        } else if(obj["command"] == "delete") {
            QString query {
                "DELETE FROM sunflower WHERE year = MAKE_DATE("
                + obj["year"].toString() + ", 1, 1);"
            };
            document = queries::doQuery(obj, query);

        } else if(obj["command"] == "table") {
            document = queries::getModel(obj);

        } else if(obj["command"] == "first") {
            document = queries::getModel(obj, "SELECT * FROM collection");

        } else if(obj["command"] == "second") {
            document = queries::getModel(obj, "SELECT * FROM average_production");
        }

        data = document.toJson(QJsonDocument::Compact);

        socket->write(crypto::encrypt(data));
        socket->flush();

        qDebug() << ">> Sent object:\n" << document;

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
