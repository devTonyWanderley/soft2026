#ifndef DXFPARSER_H
#define DXFPARSER_H

#include <QString>
#include <QStringList>
#include <QRegularExpression>
#include <QVector>

struct TopoPoint {
    QString id;
    QString attr;
    double n = 0.0;
    double e = 0.0;
    double z = 0.0;
};

class dxfparser {
public:
    // Retorna uma lista de pontos extraídos do buffer bruto
    static QVector<TopoPoint> parseLeicaDxf(const QByteArray &data);

    // Aquela sua máscara de 68 caracteres que discutimos
    static QString formatToFixedLine(const TopoPoint &p);
};

#endif // DXFPARSER_H
