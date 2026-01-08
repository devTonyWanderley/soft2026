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

    bool buscaAr2(uint &j, Nos<Ponto> *p, Nos<Ponto> *q, bool &ini);
    void geraFaces();
public:
    Superficie(const QString id = "Genérica");
    QString GNome();
    void SNome(const QString id);
    void addPonto(const QString ln);
    void addAresta(const QString ln);
    uint numPontos();
    uint numArestas();
};

#endif // SUPERFICIE_H
