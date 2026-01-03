#include "aresta.h"

Aresta::Aresta()
{
    Vini = Vfim = nullptr;
}

Aresta::Aresta(const QString ln, Nos<Ponto> *primo)
{
    Vini = GetPonto(get_Va_Ln_Ars(ln), primo);
    Vfim = GetPonto(get_Vb_Ln_Ars(ln), primo);
}

Aresta::Aresta(const Aresta &outra)
{
    Vini = outra.Vini;
    Vfim = outra.Vfim;
}

Aresta::Aresta(Nos<Ponto> *v0, Nos<Ponto> *v1)
{
    if(v0->Valor.GetZ() > v1->Valor.GetZ())
    {
        Vini = v1;
        Vfim = v0;
    }
    else
    {
        Vini = v0;
        Vfim = v1;
    }
}

Aresta::Aresta(QString v0, QString v1, Nos<Ponto> *primo)
{
    QString ig = TIPO_ALFA;
    ig += TIPO_NUM;
    v0 = IgnoreEsquerda(v0, ig);
    v1 = IgnoreEsquerda(v1, ig);
    Vini = GetPonto(v0, primo);
    Vfim = GetPonto(v1, primo);
    if(Vini->Valor.GetZ() > Vfim->Valor.GetZ())
    {
        Nos<Ponto> *pont = Vini;
        Vini = Vfim;
        Vfim = pont;
    }
}

Nos<Ponto> * Aresta::GetVini()
{
    return Vini;
}

Nos<Ponto> * Aresta::GetVfim()
{
    return Vfim;
}

Aresta Aresta::operator =(const Aresta &outra)
{
    Vini = outra.Vini;
    Vfim = outra.Vfim;
    return *this;
}

bool Aresta::operator ==(const Aresta outra)
{
    return ((Vini == outra.Vini) && (Vfim == outra.Vfim));
}
