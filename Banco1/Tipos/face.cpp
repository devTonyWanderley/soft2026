#include "face.h"

Face::Face()
{
    Va = Vb = Vc = nullptr;
}

Face::Face(const QString ln, Nos<Ponto> *primo)
{
    Va = GetPonto(get_Va_Ln_Fce(ln), primo);
    Vb = GetPonto(get_Vb_Ln_Fce(ln), primo);
    Vc = GetPonto(get_Vc_Ln_Fce(ln), primo);
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
    QString ig = TIPO_ALFA;
    ig += TIPO_NUM;
    ig += TIPO_LIGA;
    Va = GetPonto(IgnoreEsquerda(v0, ig), primo);
    Vb = GetPonto(IgnoreEsquerda(v1, ig), primo);
    Vc = GetPonto(IgnoreEsquerda(v2, ig), primo);
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
