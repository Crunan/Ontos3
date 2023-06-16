#include "include/functions.h"

functions::functions()
{

}
//VALIDATORS & CONVERTERS
//QString functions::BinInt2String(int Bits16) {
//    QString rtn;
//    Bits16 = Bits16 | 0x10000; //for bit 16 high
//    rtn = QString::number(Bits16,2);
//    return rtn.mid(1);
//}
//bool functions::StringIsValidIntChars(QString testString) const {
//    QIntValidator intValidator;
//    int necessaryVariable{0};
//    if (intValidator.validate(testString, necessaryVariable) == QIntValidator::Acceptable)
//        return true;
//    else
//        return false;
//}
//bool functions::StringIsValidDoubleChars(QString testString) const {
//    QDoubleValidator DblValidator;
//    int necessaryVariable{0};
//    return DblValidator.validate(testString, necessaryVariable);
//}
//bool functions::StringIsValidHexChars(QString testString) const {
//    QRegularExpression HexChars("0123456789ABCDEF");
//    QRegularExpressionValidator HexValidator(HexChars);
//    int necessaryVariable{0};
//    return HexValidator.validate(testString, necessaryVariable);
//}
//QString functions::DecIntToDecStr(int DecInt, int numChar) {
//    QString DecStr;
//    DecStr = DecStr.setNum(DecInt, 10);

//    while (DecStr.length() < numChar)
//        DecStr = "0" + DecStr;

//    return DecStr;
//}
//bool functions::isBitSet(int test_int, int bit_pos) {
//    int bitmask{};

//    bitmask = 1<<bit_pos;
//    if (test_int & bitmask) {
//        return true;
//    }
//    else {
//        return false;
//    }
//}
