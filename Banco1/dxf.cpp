#include "dxf.h"

dxfAtr::dxfAtr()
{
    Cod = 999999;
    Valor = "";
}

dxfAtr::dxfAtr(const uint c, const QString v)
{
    Cod = c;
    Valor = v;
}

uint dxfAtr::gCod()
{
    return Cod;
}

QString dxfAtr::gVal()
{
    return Valor;
}

dxfEnt::dxfEnt()
{
    Ent.Clear();
}

void dxfEnt::addAtr(const uint c, const QString v)
{
    Ent.Pushback(dxfAtr(c, v));
}
