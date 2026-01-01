#ifndef PONTO_H
#define PONTO_H

#include "ecivil.h"

class Ponto : public ECivil
{
private:
    QString Nome;
    QString Atri;
    double Abci;
    double Orde;
    double Cota;

    double LerDouble(QString arg);
    bool LerLinha(QString &ln, QString &id, QString &atr, double &x, double &y, double &z);
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
