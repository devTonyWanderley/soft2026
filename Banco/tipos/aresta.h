#ifndef ARESTA_H
#define ARESTA_H

#include "ponto.h"
#include "../ed/gno.h"

class Aresta : public Ponto
{
private:
    Nos<Ponto> * Vini;
    Nos<Ponto> * Vfim;

    Nos<Ponto> * GetPonto(const QString v, Nos<Ponto> *primo);
public:
    Aresta();
    Aresta(const QString ln, Nos<Ponto> *primo);
    Aresta(const Aresta &outra);
    Aresta(QString v0, QString v1, Nos<Ponto> *primo);
    Nos<Ponto> * GetVini();
    Nos<Ponto> * GetVfim();
    Aresta operator =(const Aresta &outra);
    bool operator ==(const Aresta outra);
};

#endif // ARESTA_H
