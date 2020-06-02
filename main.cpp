#include "mainwindow.h"
#include <QApplication>
//#include <iostream>


int main(int argc, char *argv[])//不需要修改
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
