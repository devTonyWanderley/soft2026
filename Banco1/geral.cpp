#include "geral.h"

//  Manipulação de texto
QString AddChr(const QString tx, const QChar c, const uint n, bool esq)
{
    if(tx.length() >= n)
    {
        if(esq)
            return tx.right(n);
        else
            return tx.left(n);
    }
    QString r = tx;
    while(r.length() < n)
        if(esq)
            r = c + r;
        else
            r += c;
    if(r.length() > n)
    {
        if(esq)
            return r.right(n);
        else
            return r.left(n);
    }
    return r;
}

QString AddChr(const QString tx, const QString c, const uint n, bool esq)
{
    if(tx.length() >= n)
    {
        if(esq)
            return tx.right(n);
        else
            return tx.left(n);
    }
    QString r = tx;
    while(r.length() < n)
        if(esq)
            r = c + r;
        else
            r += c;
    if(r.length() > n)
    {
        if(esq)
            return r.right(n);
        else
            return r.left(n);
    }
    return r;
}

QString Ignore(const QString tx, const QString exceto)
{
    QString r = "";
    for(uint i = 0; i < tx.length(); i++)
        if(exceto.indexOf(tx.at(i)) != -1)
            r += tx.at(i);
    return r;
}

QString IgnoreEsquerda(const QString tx, const QString exceto)
{
    QString r = tx;
    while((r.length()) && (exceto.indexOf(r.at(0))) == -1)
        r = r.right(r.length() - 1);
    return r;
}

QString IgnoreDireita(const QString tx, const QString exceto)
{
    QString r = tx;
    while((r.length()) && (exceto.indexOf(r.at(r.length() - 1))) == -1)
        r = r.left(r.length() - 1);
    return r;
}

QString get_Id_Ln_Pnt(const QString ln)
{
    QString ig = TIPO_ALFA;
    ig += TIPO_NUM;
    return IgnoreEsquerda(ln.left(8), ig);
}

QString get_Atr_Ln_Pnt(const QString ln)
{
    QString ig = TIPO_ALFA;
    ig += TIPO_NUM;
    ig += TIPO_LIGA;
    return IgnoreEsquerda(ln.right(ln.length() - 8).left(8), ig);
}

double get_Abci_Ln_Pnt(const QString ln)
{
    QString ig = TIPO_NUM, tx = ln.right(ln.length() - 16).left(10);
    ig = ig.right(ig.length() - 1);
    ig += '.';
    if(tx.indexOf(',') != -1)
        tx = tx.left(tx.indexOf(',')) + '.' + tx.right(tx.length() - tx.indexOf(',') - 1);
    tx = IgnoreEsquerda(tx, ig);
    return tx.toDouble();
}

double get_Orde_Ln_Pnt(const QString ln)
{
    QString ig = TIPO_NUM, tx = ln.right(ln.length() - 26).left(11);
    ig = ig.right(ig.length() - 1);
    ig += '.';
    if(tx.indexOf(',') != -1)
        tx = tx.left(tx.indexOf(',')) + '.' + tx.right(tx.length() - tx.indexOf(',') - 1);
    tx = IgnoreEsquerda(tx, ig);
    return tx.toDouble();
}

double get_Cota_Ln_Pnt(const QString ln)
{
    QString ig = TIPO_NUM, tx = ln.right(8);
    ig = ig.right(ig.length() - 1);
    ig += '.';
    if(tx.indexOf(',') != -1)
        tx = tx.left(tx.indexOf(',')) + '.' + tx.right(tx.length() - tx.indexOf(',') - 1);
    tx = IgnoreEsquerda(tx, ig);
    return tx.toDouble();
}
