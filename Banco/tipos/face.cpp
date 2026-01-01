#include "face.h"

Face::Face()
{
    Va = Vb = Vc = nullptr;
}

Face::Face(const QString ln, Nos<Ponto> *primo)
{
    QString v0 = NormaTexto(ln.left(8)), v1, v2 = ln.right(16);
    v1 = NormaTexto(v2.left(8));
    v2 = NormaTexto(v2.right(8));
    Va = GetPonto(v0, primo);
    Vb = GetPonto(v1, primo);
    Vc = GetPonto(v2, primo);
    if(Va->Valor.GetZ() > Vb->Valor.GetZ())
    {
        Nos<Ponto> *pont = Va;
        Va = Vb;
        Vb = pont;
    }
    if(Va->Valor.GetZ() > Vc->Valor.GetZ())
    {
        Nos<Ponto> *pont = Va;
        Va = Vc;
        Vc = pont;
    }
    if(Vb->Valor.GetZ() > Vc->Valor.GetZ())
    {
        Nos<Ponto> *pont = Vb;
        Vb = Vc;
        Vc = pont;
    }
}

Face::Face(const Face &outra)
{
    Va = outra.Va;
    Vb = outra.Vb;
    Vc = outra.Vc;
}

Face::Face(Nos<Ponto> *v0, Nos<Ponto> *v1, Nos<Ponto> *v2)
{
    Va = v0;
    Vb = v1;
    Vc = v2;
    if(Va->Valor.GetZ() > Vb->Valor.GetZ())
    {
        Nos<Ponto> *pont = Va;
        Va = Vb;
        Vb = pont;
    }
    if(Va->Valor.GetZ() > Vc->Valor.GetZ())
    {
        Nos<Ponto> *pont = Va;
        Va = Vc;
        Vc = pont;
    }
    if(Vb->Valor.GetZ() > Vc->Valor.GetZ())
    {
        Nos<Ponto> *pont = Vb;
        Vb = Vc;
        Vc = pont;
    }
}

Face::Face(QString v0, QString v1, QString v2, Nos<Ponto> *primo)
{
    Va = GetPonto(NormaTexto(v0), primo);
    Vb = GetPonto(NormaTexto(v1), primo);
    Vc = GetPonto(NormaTexto(v2), primo);
    if(Va->Valor.GetZ() > Vb->Valor.GetZ())
    {
        Nos<Ponto> *pont = Va;
        Va = Vb;
        Vb = pont;
    }
    if(Va->Valor.GetZ() > Vc->Valor.GetZ())
    {
        Nos<Ponto> *pont = Va;
        Va = Vc;
        Vc = pont;
    }
    if(Vb->Valor.GetZ() > Vc->Valor.GetZ())
    {
        Nos<Ponto> *pont = Vb;
        Vb = Vc;
        Vc = pont;
    }
}

Nos<Ponto> * Face::GetPonto(const QString v, Nos<Ponto> *primo)
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

Nos<Ponto> * Face::GetVa()
{
    return Va;
}

Nos<Ponto> * Face::GetVb()
{
    return Vb;
}

Nos<Ponto> * Face::GetVc()
{
    return Vc;
}

Face Face::operator =(const Face &outra)
{
    Va = outra.Va;
    Vb = outra.Vb;
    Vc = outra.Vc;
    return *this;
}

bool Face::operator ==(const Face outra)
{
    if(Va != outra.Va) return false;
    if(Vb != outra.Vb) return false;
    return (Vc == outra.Vc);
}
