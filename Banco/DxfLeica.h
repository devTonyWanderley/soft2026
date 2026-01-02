#ifndef DXFLEICA_H
#define DXFLEICA_H

#include <QString>
#include "geral.h"

bool RemoveEsquerda(QString &arg, const QString bnd);
bool SeparaEntities(QString &arg);
bool LerProxPonto(QString &arg, QString &id, QString &atr, QString &x, QString &y, QString &z);

#endif // DXFLEICA_H
