#include "form.h"
#include "ui_form.h"
#include "connect.h"

#include <QMessageBox>

Form::Form(MainWindow& window, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Form)
{
    ui->setupUi(this);

    QObject::connect(ui->buttonLogin, &QPushButton::clicked, this, [this, &window](){
        QJsonObject jsonObject {};
        jsonObject["command"] = "login";
        jsonObject["username"] = ui->lineUsername->text();
        jsonObject["password"] = ui->linePassword->text();

        QJsonDocument jsonDocument(jsonObject);
        QByteArray jsonUtf8(jsonDocument.toJson(QJsonDocument::Compact));

        if (!connect::login(jsonUtf8)) {
            QMessageBox::warning(nullptr, "Warning", "Cannot login!");
        } else {
            window.setJsonObject(jsonObject);
            window.setForm(this);
            window.changeModel("table");
            window.show();
            hide();
        }
    });
}

Form::~Form()
{
    delete ui;
}
