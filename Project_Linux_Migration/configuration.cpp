#include "configuration.h"

QString Configuration::getMFC1() const {
    return MFC1;
}

QString Configuration::getMFC2() const {
    return MFC2;
}

QString Configuration::getMFC3() const {
    return MFC3;
}

QString Configuration::getMFC4() const {
    return MFC4;
}

void Configuration::setMFC1(const QString& newMFC1) {
    MFC1 = newMFC1;
}

void Configuration::setMFC2(const QString& newMFC2) {
    MFC2 = newMFC2;
}

void Configuration::setMFC3(const QString& newMFC3) {
    MFC3 = newMFC3;
}

void Configuration::setMFC4(const QString& newMFC4) {
    MFC4 = newMFC4;
}

QString Configuration::getExeConfigPath() const {
    return ExeConfigPath;
}

QString Configuration::getExeConfigPathFileName() const {
    return ExeConfigPathFileName;
}
void Configuration::GetExeCfg() {
    QStringList Values;
    QString filePath = "/home/oem/Ontos3/Project_Linux_Migration/config/default.cfg";

    // Check if the file exists
    if(!QFileInfo::exists(filePath)) {
        QMessageBox::information(0, "Error", "Configuration file does not exist");
        return;
    }

    QFile file(filePath);

    //Try to open the file
    if (!file.open(QIODevice::ReadOnly)) {QTextStream in(&file);
        QMessageBox::information(0, "Error", file.errorString());
        return;
    }

    QTextStream in(&file);

    while(!in.atEnd()) {
        QString line = in.readLine();
        Values += line.split(">");
    }

    file.close();

    loadConfigGUI(Values);

}
