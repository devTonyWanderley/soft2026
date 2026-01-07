#ifndef DXF_H
#define DXF_H

#include <QString>
#include "../Banco1/ed/glista.h"

class dxfAtr
{
private:
    uint Cod;
    QString Valor;
public:
    dxfAtr();
    dxfAtr(const uint c, const QString v);
    uint gCod();
    QString gVal();
};

class dxfEnt
{
public:
    Lista<dxfAtr> Ent;
    dxfEnt();
    void addAtr(const uint c, const QString v);
};

#endif // DXF_H
