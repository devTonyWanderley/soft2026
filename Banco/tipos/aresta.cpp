#include "aresta.h"

Aresta::Aresta()
{
    Vini = Vfim = nullptr;
}

Aresta::Aresta(const QString ln, Nos<Ponto> *primo)
{
    QString va = NormaTexto(ln.left(8)), vb = NormaTexto(ln.right(8));
    Vini = GetPonto(va, primo);
    Vfim = GetPonto(vb, primo);
    if(Vini->Valor.GetZ() > Vfim->Valor.GetZ())
    {
        Nos<Ponto> *pont = Vini;
        Vini = Vfim;
        Vfim = pont;
    }
}

Aresta::Aresta(const Aresta &outra)
{
    Vini = outra.Vini;
    Vfim = outra.Vfim;
}

Aresta::Aresta(QString v0, QString v1, Nos<Ponto> *primo)
{
    v0 = NormaTexto(v0);
    v1 = NormaTexto(v1);
    Vini = GetPonto(v0, primo);
    Vfim = GetPonto(v1, primo);
    if(Vini->Valor.GetZ() > Vfim->Valor.GetZ())
    {
        Nos<Ponto> *pont = Vini;
        Vini = Vfim;
        Vfim = pont;
    }
}

Nos<Ponto> * Aresta::GetPonto(const QString v, Nos<Ponto> *primo)
{
    Nos<Ponto> *pont = primo;
    while(pont)
    {
        if(pont->Valor.GetId() == v)
        {
            return pont;
        }
        pont = pont->Segue;
    }
    return nullptr;
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
