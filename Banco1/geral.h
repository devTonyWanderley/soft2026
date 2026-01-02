#ifndef GERAL_H
#define GERAL_H

#define TIPO_ALFA "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define TIPO_NUM "0123456789"
#define TIPO_SEP_NUM ",."
#define TIPO_LIGA "-_<>"

#include <QString>
#include <QDebug>

/*GERAL_H
    QString fillEsquerda(QString tx, QChar c, uint n);  ..  ..  ..  ..  ..  ..  ..  ..  ..  ..  sub por AddChr
    QString fillDireita(QString tx, QChar c, uint n);  ..  ..  ..  ..  ..  ..  ..  ..  ..  ..  sub por AddChr
QString fillNumero(QString tx, uint ni, uint nf);
//DXFLEICA_H
    bool RemoveEsquerda(QString &arg, const QString bnd);
bool SeparaEntities(QString &arg);
bool LerProxPonto(QString &arg, QString &id, QString &atr, QString &x, QString &y, QString &z);
//ECIVIL_H
    QString NormaTexto(QString arg);................................................................................
QString FillPreTexto(QString tx, QChar c, uint n);  ..  ..  ..  ..  ..  ..  ..  ..  ..  ..  sub por AddChr
  ARESTA_H, FACE_H, PONTO_H
            Aresta(const QString ln, Nos<Ponto> *primo);
Face(const QString ln, Nos<Ponto> *primo);
Ponto(const QString ln);
*/

//  Manipulação de texto:
QString AddChr(const QString tx, const QChar c, const uint n, bool esq);
QString AddChr(const QString tx, const QString c, const uint n, bool esq);

QString Ignore(const QString tx, const QString exceto);
QString IgnoreEsquerda(const QString tx, const QString exceto);
QString IgnoreDireita(const QString tx, const QString exceto);

QString get_Id_Ln_Pnt(const QString ln);
QString get_Atr_Ln_Pnt(const QString ln);
double get_Abci_Ln_Pnt(const QString ln);
double get_Orde_Ln_Pnt(const QString ln);
double get_Cota_Ln_Pnt(const QString ln);

#endif // GERAL_H
