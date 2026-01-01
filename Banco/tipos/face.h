#ifndef FACE_H
#define FACE_H

#include "ponto.h"
#include "../ed/gno.h"

class Face : public Ponto
{
private:
    Nos<Ponto> * Va;
    Nos<Ponto> * Vb;
    Nos<Ponto> * Vc;

    Nos<Ponto> * GetPonto(const QString v, Nos<Ponto> *primo);
public:
    Face();
    Face(const QString ln, Nos<Ponto> *primo);
    Face(const Face &outra);
    Face(Nos<Ponto> *v0, Nos<Ponto> *v1, Nos<Ponto> *v2);
    Face(QString v0, QString v1, QString v2, Nos<Ponto> *primo);
    Nos<Ponto> * GetVa();
    Nos<Ponto> * GetVb();
    Nos<Ponto> * GetVc();
    Face operator =(const Face &outra);
    bool operator ==(const Face outra);
};

#endif // FACE_H
