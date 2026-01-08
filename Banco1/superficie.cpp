#include "superficie.h"

Superficie::Superficie(const QString id)
{
    Nome = id;
    Pontos.Clear();
    Arestas.Clear();
    Faces.Clear();
    Contorno.Clear();
}

QString Superficie::GNome()
{
    return Nome;
}

void Superficie::SNome(const QString id)
{
    Nome = id;
}

void Superficie::addPonto(const QString ln)
{
    Pontos.Pushback(Ponto(ln));
}

void Superficie::addAresta(const QString ln)
{
    Arestas.Pushback(Aresta(ln, Pontos.GetPrimo()));
}

void Superficie::addFace(const uint ii, const uint jj)
{
    Nos<Ponto> *p = Arestas.Getn(ii).GetVini(), *q = Arestas.Getn(ii).GetVfim(), *r, *s;
    if(p == Arestas.Getn(jj).GetVini() || q == Arestas.Getn(jj).GetVini())
        r = Arestas.Getn(jj).GetVfim();
    else
        r = Arestas.Getn(jj).GetVini();
    if(p->Valor.GetZ() > q->Valor.GetZ())
    {
        s = p;
        p = q;
        q = s;
    }
    if(p->Valor.GetZ() > r->Valor.GetZ())
    {
        s = p;
        p = r;
        r = s;
    }
    if(q->Valor.GetZ() > r->Valor.GetZ())
    {
        s = q;
        q = r;
        r = s;
    }
    Faces.Pushback(Face(p, q, r));
}

uint Superficie::numPontos()
{
    return Pontos.Length();
}

uint Superficie::numArestas()
{
    return Arestas.Length();
}

uint Superficie::numFaces()
{
    return Faces.Length();
}

bool Superficie::buscaAr2(const uint i, uint &j)
{
    while(j < (Arestas.Length() - 1))
    {
        if((Arestas.Getn(i).GetVini() == Arestas.Getn(j).GetVini()) ||
            (Arestas.Getn(i).GetVini() == Arestas.Getn(j).GetVfim()) ||
            (Arestas.Getn(i).GetVfim() == Arestas.Getn(j).GetVfim()) ||
            (Arestas.Getn(i).GetVfim() == Arestas.Getn(j).GetVini())
            )
            return true;
        j++;
    }
    return false;
}

bool Superficie::buscaAr3(const uint i, const uint j, uint &k)
{
    k = j + 1;
    Nos<Ponto> *p, *q;
    if(Arestas.Getn(i).GetVini() == Arestas.Getn(j).GetVini())
    {
        p = Arestas.Getn(i).GetVfim();
        q = Arestas.Getn(j).GetVfim();
    }
    else if(Arestas.Getn(i).GetVini() == Arestas.Getn(j).GetVfim())
    {
        p = Arestas.Getn(i).GetVfim();
        q = Arestas.Getn(j).GetVini();
    }
    else if(Arestas.Getn(i).GetVfim() == Arestas.Getn(j).GetVfim())
    {
        p = Arestas.Getn(i).GetVini();
        q = Arestas.Getn(j).GetVini();
    }
    else
    {
        p = Arestas.Getn(i).GetVini();
        q = Arestas.Getn(j).GetVfim();
    }
    while(k < Arestas.Length())
    {
        if(
            (p == Arestas.Getn(k).GetVini() && q == Arestas.Getn(k).GetVfim()) ||
            (p == Arestas.Getn(k).GetVfim() && q == Arestas.Getn(k).GetVini())
            )
            return true;
        k++;
    }
    return false;
}

void Superficie::geraFaces()
{
    uint i = 0, j, k, c;
    while(i < (Arestas.Length() - 2))
    {
        j = i + 1;
        c = 0;
        while(buscaAr2(i, j) && (c < 2))
        {
            k = j + 1;
            if(buscaAr3(i, j, k))
            {
                addFace(i,j);
                c++;
            }
            j++;
        }
        i++;
    }
}
