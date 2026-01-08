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

    bool buscaAr2(const uint i, uint &j);
    bool buscaAr3(const uint i, const uint j, uint &k);
public:
    Superficie(const QString id = "Genérica");
    QString GNome();
    void SNome(const QString id);
    void addPonto(const QString ln);
    void addAresta(const QString ln);
    void addFace(const uint ii, const uint jj);
    uint numPontos();
    uint numArestas();
    uint numFaces();
    void geraFaces();
};

#endif // SUPERFICIE_H
