#ifndef PONTO_H
#define PONTO_H

#include "../geral.h"

class Ponto
{
private:
    QString Nome;
    QString Atri;
    double Abci;
    double Orde;
    double Cota;
public:
    Ponto();
    Ponto(const QString ln);
    Ponto(const Ponto& outro);
    Ponto(const QString id, const QString atr, const double x, const double y, const double z);
    void SetId(QString id);
    void SetAtr(QString atr);
    void SetX(double x);
    void SetY(double y);
    void SetZ(double z);
    QString GetId();
    QString GetAtr();
    double GetX();
    double GetY();
    double GetZ();
    Ponto operator = (const Ponto &outro);
    bool operator == (const Ponto outro);
};

#endif // PONTO_H
