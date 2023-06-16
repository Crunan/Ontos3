#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#pragma once

#include "include/mainloop.h"
#include "include/settingsdialog.h"
#include "include/recipe.h"ontos3
#include "include/logger.h"
#include "include/serialcomms.h"
#include <QMainWindow>
#include <QMessageBox>


namespace Ui {
class MainWindow;
}

QT_END_NAMESPACE

class MainWindow : public QMainWindow
{

    Q_OBJECT

public:
    explicit MainWindow(MainLoop& loop, Logger& logger, QWidget *parent = nullptr);
    ~MainWindow();

    Recipe* getRecipe() const;
    void createRecipe();

private:
    Ui::MainWindow *ui = nullptr;
    QLabel* status = nullptr;
    SettingsDialog* settings = nullptr;
    // Reference to the MainLoop object
    MainLoop& mainLoop;
    // Reference to the logger
    Logger& log;
    // Pointer to Serial Communications
    SerialComms* serial;
    // Pointer to the Recipe object
    Recipe* recipe;

    // Action Button methods
    void initActionsConnections();
    void showStatusMessage(const QString &message);
    void about();
    void shutDownProgram();
    // Serial Port methodsRecipe*
    void SerialPortConnectedSetupFunctions();
    void adjustButtonAvailabilityOnSerialConnect();
    void displayStatusSerialPortConnected();
    // Recipe methods
    void createRecipe();
    Recipe* getRecipe() const;


private slots:

};
#endif // MAINWINDOW_H
