#include "include/configuration.h"

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
//void Configuration::GetExeCfg() {
//    QStringList Values;
//    QString filePath = "/home/oem/Ontos3/Project_Linux_Migration/config/default.cfg";

//    // Check if the file exists
//    if(!QFileInfo::exists(filePath)) {
//        QMessageBox::information(0, "Error", "Configuration file does not exist");
//        return;
//    }

//    QFile file(filePath);

//    //Try to open the file
//    if (!file.open(QIODevice::ReadOnly)) {QTextStream in(&file);
//        QMessageBox::information(0, "Error", file.errorString());
//        return;
//    }

//    QTextStream in(&file);

//    while(!in.atEnd()) {
//        QString line = in.readLine();
//        Values += line.split(">");
//    }

//    file.close();

//    loadConfigGUI(Values);

//}

//struct configuration {
//    QString ExeConfigPath = "./config/";
//    QString ExeConfigPathFileName = "default";
//    QString MFC1;
//    QString MFC2;
//    QString MFC3;
//    QString MFC4;

//    public:
//    QString getMFC1() const;
//    QString getMFC2() const;
//    QString getMFC3() const;
//    QString getMFC4() const;

//    void setMFC1(const QString &newMFC1);
//    void setMFC2(const QString &newMFC2);
//    void setMFC3(const QString &newMFC3);
//    void setMFC4(const QString &newMFC4);
//    QString getExeConfigPath() const;
//    QString getExeConfigPathFileName() const;

//} config;

//QString configuration::getMFC2() const
//{
//    return MFC2;
//}

//void configuration::setMFC2(const QString &newMFC2)
//{
//    MFC2 = newMFC2;
//}

//QString configuration::getMFC1() const
//{
//    return MFC1;
//}

//void configuration::setMFC1(const QString &newMFC1)
//{
//    MFC1 = newMFC1;
//}

//QString configuration::getMFC3() const
//{
//    return MFC3;
//}

//void configuration::setMFC3(const QString &newMFC3)
//{
//    MFC3 = newMFC3;
//}

//QString configuration::getMFC4() const
//{
//    return MFC4;
//}

//void configuration::setMFC4(const QString &newMFC4)
//{
//    MFC4 = newMFC4;
//}

//QString configuration::getExeConfigPath() const
//{
//    return ExeConfigPath;
//}

//QString configuration::getExeConfigPathFileName() const
//{
//    return ExeConfigPathFileName;
//}
