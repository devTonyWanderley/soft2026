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
