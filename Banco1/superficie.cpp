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

uint Superficie::numPontos()
{
    return Pontos.Length();
}

uint Superficie::numArestas()
{
    return Arestas.Length();
}

void Superficie::geraFaces()
{
    uint i = 0, j, k;
    Lista<Aresta> cont;
    Aresta ar0, ar1, ar2;
    Nos<Ponto> *p, *q;
    while(i < Arestas.Length())
    {
        ar0 = Arestas.Getn(i);
        j = i + 1;
        cont = 0;
        while(j < Arestas.Length())
        {
            ar1 = Arestas.Getn(j);
            p = q = nullptr;
            if(ar0.GetVini() == ar1.GetVini())
            {
                p = ar0.GetVfim();
                q = ar1.GetVfim();
            }
            else if(ar0.GetVini() == ar1.GetVfim()) // parou por aqui
            {
                p = ar0.GetVfim();
                q = ar1.GetVfim();
            }
        }
    }
}
