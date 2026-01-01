#include "ponto.h"

Ponto::Ponto()
{
    Nome = Atri = "";
    Abci = Orde = Cota = 0;
}

Ponto::Ponto(const QString ln)
{
    QString lnl = ln, idl, atrl;
    double xl, yl, zl;
    if(LerLinha(lnl,idl, atrl, xl, yl, zl))
    {
        Nome = idl;
        Atri = atrl;
        Abci = xl;
        Orde = yl;
        Cota = zl;
    }
    else
    {
        Nome = Atri = "";
        Abci = Orde = Cota = 0;
    }
}

Ponto::Ponto(const Ponto& outro)
{
    Nome = outro.Nome;
    Atri = outro.Atri;
    Abci = outro.Abci;
    Orde = outro.Orde;
    Cota = outro.Cota;
}

Ponto::Ponto(const QString id, const QString atr, const double x, const double y, const double z)
{
    Nome = id;
    Atri = atr;
    Abci = x;
    Orde = y;
    Cota = z;
}

double Ponto::LerDouble(QString arg)
{
    QString ta = arg.left(arg.length() - 4), tb = arg.right(3);
    double a = ta.toDouble(), b = tb.toDouble();
    b /= 1000.0;
    return (a + b);
}

bool Ponto::LerLinha(QString &ln, QString &id, QString &atr, double &x, double &y, double &z)
{
    if(ln.length() != 45) return false;
    id = ln.left(8);
    ln = ln.right(37);
    atr = ln.left(8);
    ln = ln.right(29);
    x = LerDouble(ln.left(10));
    ln = ln.right(19);
    y = LerDouble(ln.left(11));
    z = LerDouble(ln.right(8));
    id = NormaTexto(id);
    atr = NormaTexto(atr);
    return true;
}

void Ponto::SetId(QString id)
{
    Nome = id;
}

void Ponto::SetAtr(QString atr)
{
    Atri = atr;
}

void Ponto::SetX(double x)
{
    Abci = x;
}

void Ponto::SetY(double y)
{
    Orde = y;
}

void Ponto::SetZ(double z)
{
    Cota = z;
}

QString Ponto::GetId()
{
    return Nome;
}

QString Ponto::GetAtr()
{
    return Atri;
}

double Ponto::GetX()
{
    return Abci;
}

double Ponto::GetY()
{
    return Orde;
}

double Ponto::GetZ()
{
    return Cota;
}

Ponto Ponto::operator = (const Ponto &outro)
{
    Nome = outro.Nome;
    Atri = outro.Atri;
    Abci = outro.Abci;
    Orde = outro.Orde;
    Cota = outro.Cota;
    return *this;
}

bool Ponto::operator == (const Ponto outro)
{
    if(Abci != outro.Abci) return false;
    if(Orde != outro.Orde) return false;
    return (Cota == outro.Cota);
}
