#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <QObject>
#include <QString>
#include <QIntValidator>
#include <QDoubleValidator>
#include <QRegularExpression>
#include <QRegularExpressionValidator>

class functions : public QObject
{
    Q_OBJECT

public:
    functions();

    QString BinInt2String(int Bits16);
    bool StringIsValidIntChars(QString testString) const;
    bool StringIsValidDoubleChars(QString testString) const;
    bool StringIsValidHexChars(QString testString) const;
    QString DecIntToDecStr(int DecInt, int numChar);
    bool isBitSet(int test_int, int bit_pos);
};

#endif // FUNCTIONS_H
