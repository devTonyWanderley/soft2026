#ifndef ECIVIL_H
#define ECIVIL_H

#include <QString>

class ECivil
{
public:
    ECivil();
    QString NormaTexto(QString arg);
    QString FillPreTexto(QString tx, QChar c, uint n);
};

#endif // ECIVIL_H
