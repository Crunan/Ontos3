#include <QtWidgets/QApplication>
#include "mainwindow.h"
#include "mainloop.h"
#include "logger.h"
#include "runguard.h"

#include "ui_mainwindow.h"


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // Single instance protection
    runguard guard("Ontos3");
    if (!guard.tryToRun()) {
        QMessageBox msgBox; // must run after instantiation of QApplication
        msgBox.setText(QCoreApplication::tr("Ontos Plasma is already running"));
        msgBox.setInformativeText(QCoreApplication::tr("Another instance of Ontos Plasma is running, please close that instance before running another."));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
        return -1; // non zero return code
    }

    // these allow us to use the QSettings from multiple places in the app
    // using the default constructor
    QCoreApplication::setOrganizationName("OntosPlasma");
    QCoreApplication::setOrganizationDomain("OntosPlasma.com");
    QCoreApplication::setApplicationName("Ontos3");

    // Create our main loop
    // Contains our Event loop and Timer
    MainLoop *mainLoop = new MainLoop(); // cleaned up in MainWindow destructor
    mainLoop->start();

    // Initialize our logger
    Logger::init();

    Logger::logInfo("Current application version: " + QString(APP_VERSION));

    // Create our main window
    // Contains our GUI
    MainWindow window(mainLoop);
    window.show();

    //end program
    bool state = a.exec();
    return state;
}

