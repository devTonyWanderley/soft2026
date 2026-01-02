#include "ponto.h"

Ponto::Ponto()
{
    Nome = Atri = "";
    Abci = Orde = Cota = 0;
}

Ponto::Ponto(const QString ln)
{
    Nome = get_Id_Ln_Pnt(ln);
    Atri = get_Atr_Ln_Pnt(ln);
    Abci = get_Abci_Ln_Pnt(ln);
    Orde = get_Orde_Ln_Pnt(ln);
    Cota = get_Cota_Ln_Pnt(ln);
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
