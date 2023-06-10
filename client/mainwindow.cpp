#include "connect.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , model(new QStandardItemModel)
{
    ui->setupUi(this);

    QObject::connect(ui->buttonInsert, &QPushButton::clicked, this, [this](){
        jsonObject["command"] = "insert";
        jsonObject["area"] = ui->lineArea->text();
        jsonObject["production"] = ui->lineProduction->text();
        jsonObject["year"] = ui->lineYear->text();

        doQuery(jsonObject);
    });

    QObject::connect(ui->buttonUpdate, &QPushButton::clicked, this, [this](){
        jsonObject["command"] = "update";
        jsonObject["area"] = ui->lineArea->text();
        jsonObject["production"] = ui->lineProduction->text();
        jsonObject["year"] = ui->lineYear->text();

        doQuery(jsonObject);
    });

    QObject::connect(ui->buttonDelete, &QPushButton::clicked, this, [this](){
        jsonObject["command"] = "delete";
        jsonObject["area"] = ui->lineArea->text();
        jsonObject["production"] = ui->lineProduction->text();
        jsonObject["year"] = ui->lineYear->text();

        doQuery(jsonObject);
    });

    QObject::connect(ui->buttonTable, &QPushButton::clicked, this, [this](){
        jsonObject["command"] = "table";

        doQuery(jsonObject);
    });
}

MainWindow::~MainWindow()
{
    delete model;
    delete ui;
}

void MainWindow::setJsonObject(QJsonObject jsonObject) {
    this->jsonObject = jsonObject;
}

void MainWindow::doQuery(QJsonObject jsonObject) {
    QJsonDocument jsonDocument(jsonObject);
    QByteArray jsonUtf8(jsonDocument.toJson(QJsonDocument::Compact));

    connect::query(model, jsonUtf8);
    ui->tableView->setModel(model);
}


