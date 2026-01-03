#ifndef GERAL_H
#define GERAL_H

#define TIPO_ALFA "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define TIPO_NUM "0123456789"
#define TIPO_SEP_NUM ",."
#define TIPO_LIGA "-_<>"

#include <QString>
#include <QDebug>

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

QString get_Va_Ln_Ars(const QString ln);
QString get_Vb_Ln_Ars(const QString ln);

QString get_Va_Ln_Fce(const QString ln);
QString get_Vb_Ln_Fce(const QString ln);
QString get_Vc_Ln_Fce(const QString ln);

#endif // GERAL_H
