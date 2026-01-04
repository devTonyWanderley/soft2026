#ifndef SUPERFICIE_H
#define SUPERFICIE_H

#include "Tipos/aresta.h"
#include "Tipos/face.h"

class Superficie
{
private:
    QString Nome;
    Lista<Ponto> Pontos;
    Lista<Aresta> Arestas;
    Lista<Face> Faces;
    Lista<QString> Contorno;
public:
    Superficie(const QString id = "Genérica");
    QString GNome();
    void SNome(const QString id);
};

#endif // SUPERFICIE_H
