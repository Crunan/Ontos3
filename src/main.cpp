#include <QtWidgets/QApplication>
#include "include/mainwindow.h"
#include "include/mainloop.h"
#include "logger.h"

#include "ui_mainwindow.h"


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // Create our main loop
    // Contains our Event loop and Timer
    MainLoop *mainLoop = new MainLoop(); // cleaned up in MainWindow destructor
    mainLoop->start();

    // Initialize our logger
    Logger::init();

    // Create our main window
    // Contains our GUI
    MainWindow window(mainLoop);
    window.show();

    //end program
    bool state = a.exec();
    return state;
}

