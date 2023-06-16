#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <QString>

struct Configuration {
    QString ExeConfigPath = "./config/";
    QString ExeConfigPathFileName = "default";
    QString MFC1;
    QString MFC2;
    QString MFC3;
    QString MFC4;

public:
    QString getMFC1() const;
    QString getMFC2() const;
    QString getMFC3() const;
    QString getMFC4() const;

    void setMFC1(const QString& newMFC1);
    void setMFC2(const QString& newMFC2);
    void setMFC3(const QString& newMFC3);
    void setMFC4(const QString& newMFC4);
    QString getExeConfigPath() const;
    QString getExeConfigPathFileName() const;
};

#endif // CONFIGURATION_H
