#ifndef GERAL_H
#define GERAL_H

#define IDIR "C:\\2026\\Soft\\Inst\\"

#include <QString>
#include <QFile>
#include <QFileDialog>

//  completar texto de tamanho fixo à esquerda, à direita e normalizar número
QString fillEsquerda(QString tx, QChar c, uint n);
QString fillDireita(QString tx, QChar c, uint n);
QString fillNumero(QString tx, uint ni, uint nf);

bool Importa(QString &buf, QString dir, QString filtro, QString titulo);
bool Importa(QString &buf, QString filtro, QString titulo);
bool Exporta(QString &buf, QString dir, QString filtro, QString titulo);
bool Exporta(QString &buf, QString filtro, QString titulo);

#endif // GERAL_H
