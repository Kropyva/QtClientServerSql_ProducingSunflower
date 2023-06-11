#include "mainwindow.h"
#include "form.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow window {};
    Form form(window);

    form.show();

    return a.exec();
}
