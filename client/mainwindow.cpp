#include "connect.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , model(new QStandardItemModel)
    , validNumeric(new QRegularExpressionValidator(QRegularExpression(R"(^(\d{1,3}(\.\d{2})?)$)"), this))
    , validYear(new QRegularExpressionValidator(QRegularExpression(R"(^(\d{4})$)"), this))
    , interact(true)
{
    ui->setupUi(this);

    ui->lineArea->setValidator(validNumeric);
    ui->lineProduction->setValidator(validNumeric);
    ui->lineYear->setValidator(validYear);

    QObject::connect(ui->buttonQuit, &QPushButton::clicked, this, [this](){
        form->show();
        hide();
    });

    QObject::connect(ui->tableView->verticalHeader(), &QHeaderView::sectionClicked, this, [this](int row){
        selectRow(row);
    });

    QObject::connect(ui->tableView, &QTableView::clicked, this, [this](const QModelIndex& index){
        selectRow(index.row());
    });

    QObject::connect(ui->buttonInsert, &QPushButton::clicked, this, [this](){
        doQuery("insert");
    });

    QObject::connect(ui->buttonUpdate, &QPushButton::clicked, this, [this](){
        doQuery("update");
    });

    QObject::connect(ui->buttonDelete, &QPushButton::clicked, this, [this](){
        if (ui->lineYear->text().size() < 4) {
            QMessageBox::warning(nullptr, "Warning", "The year line is wrong!");
        } else {
            changeModel("delete");
        }
    });

    QObject::connect(ui->buttonTable, &QPushButton::clicked, this, [this](){
        interact = true;
        changeModel("table");
    });

    QObject::connect(ui->buttonFirstTask, &QPushButton::clicked, this, [this](){
        interact = false;
        changeModel("first");
    });

    QObject::connect(ui->buttonSecondTask, &QPushButton::clicked, this, [this](){
        interact = false;
        changeModel("second");
    });
}

MainWindow::~MainWindow()
{
    delete validNumeric;
    delete validYear;
    delete model;
    delete ui;
}

void MainWindow::doQuery(const QString& command) {
    if (restrict()) {
        QMessageBox::warning(nullptr, "Warning", "The fields are wrong filled!");
        return;
    }

    changeModel(command);
}

void MainWindow::changeModel(const QString& command) {
    setJsonObject(command);

    QJsonDocument document(json);
    QByteArray utf8(document.toJson(QJsonDocument::Compact));

    if(!connect::setModel(model, utf8)) {
        QMessageBox::warning(nullptr, "Warning", "Cannot do this thing!");
        return;
    }

    ui->tableView->setModel(model);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void MainWindow::setJsonObject(QJsonObject jsonObject) {
    this->json = jsonObject;
}

void MainWindow::setJsonObject(const QString& command) {
    json["command"] = command;
    json["area"] = ui->lineArea->text();
    json["production"] = ui->lineProduction->text();
    json["year"] = ui->lineYear->text();
}

bool MainWindow::restrict() const {
    if (ui->lineArea->text().size() < 1) {
        return true;
    }

    if (ui->lineProduction->text().size() < 1) {
        return true;
    }

    if (ui->lineYear->text().size() < 4) {
        return true;
    }

    return false;
}

void MainWindow::selectRow(int row) {
    if (!interact) {
        ui->lineArea->clear();
        ui->lineProduction->clear();
        ui->lineYear->clear();
        return;
    }

    QAbstractItemModel *model = ui->tableView->model();

    int columnCount(model->columnCount());

    QVector<QVariant> fields;
    for(int column(0); column < columnCount; column++) {
        QVariant field(model->data(model->index(row, column)));
        fields.append(field);
    }

    ui->lineArea->setText(fields[0].toString());
    ui->lineProduction->setText(fields[1].toString());
    ui->lineYear->setText(fields[2].toString());
}

void MainWindow::setForm(QWidget *form) {
    this->form = form;
}

