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
    Nos<Ponto> *p, *q, *s, *r; // s é o comum. pode ser igual a p ou igual a q
    while(i < Arestas.Length())
    {
        p = Arestas.Getn(i).GetVini();
        q = Arestas.Getn(i).GetVfim();
        j = i + 1;
    }
}
