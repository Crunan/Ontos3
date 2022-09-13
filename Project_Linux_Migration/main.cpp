#include <QtWidgets/QApplication>
#include "mainwindow.h"
#include "ui_mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;    

    w.show();

    bool state = a.exec();

    //end program
    return state;
}

