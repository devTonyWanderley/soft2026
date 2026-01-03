#ifndef ARESTA_H
#define ARESTA_H

#include "../edsup.h"

class Aresta
{
private:
    Nos<Ponto> * Vini;
    Nos<Ponto> * Vfim;
public:
    Aresta();
    Aresta(const QString ln, Nos<Ponto> *primo);
    Aresta(const Aresta &outra);
    Aresta(Nos<Ponto> *v0, Nos<Ponto> *v1);
    Aresta(QString v0, QString v1, Nos<Ponto> *primo);
    Nos<Ponto> * GetVini();
    Nos<Ponto> * GetVfim();
    Aresta operator =(const Aresta &outra);
    bool operator ==(const Aresta outra);
};

#endif // ARESTA_H
