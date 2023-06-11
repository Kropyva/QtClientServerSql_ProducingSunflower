#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QJsonObject>
#include <QStandardItemModel>
#include <QRegularExpressionValidator>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void changeModel(const QString& command);
    void setJsonObject(QJsonObject jsonObject);
    void setForm(QWidget* form);

private:
    void doQuery(const QString& command);
    void setJsonObject(const QString& command);
    void selectRow(int row);
    bool restrict() const;

private:
    Ui::MainWindow *ui;
    QStandardItemModel *model;
    QRegularExpressionValidator *validNumeric;
    QRegularExpressionValidator *validYear;
    QWidget *form;
    QJsonObject json;
    bool interact;
};
#endif // MAINWINDOW_H
