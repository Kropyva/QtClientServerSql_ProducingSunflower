#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QJsonObject>
#include <QStandardItemModel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setJsonObject(QJsonObject jsonObject);
    void doQuery(QJsonObject jsonObject);

private:
    Ui::MainWindow *ui;
    QStandardItemModel *model;
    QJsonObject jsonObject;
};
#endif // MAINWINDOW_H
