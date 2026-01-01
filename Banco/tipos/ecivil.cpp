#include "ecivil.h"

ECivil::ECivil() {}

QString ECivil::NormaTexto(QString arg)
{
    QChar c;
    bool flg = true;
    while(flg)
    {
        c = *arg.data();
        if((c >= '0' && c <='9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) flg = false;
        else arg = arg.right(arg.length() - 1);
    }
    return arg;
}

QString ECivil::FillPreTexto(QString tx, QChar c, uint n)
{
    while(tx.length() < n) tx = c + tx;
    return tx;
}
