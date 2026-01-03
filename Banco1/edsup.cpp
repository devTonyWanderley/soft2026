#include "edsup.h"

Nos<Ponto> *GetPonto(const QString v, Nos<Ponto> *primo)
{
    Nos<Ponto> *pont = primo;
    while(pont)
    {
        if(pont->Valor.GetId() == v)
            break;
        pont = pont->Segue;
    }
    return pont;
}
