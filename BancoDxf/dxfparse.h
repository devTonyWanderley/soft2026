/**
 * @file dxfparser.h
 * @brief Módulo de Conversão Geométrica: DXF Leica TC 407 -> Padrão PNT (68 chars).
 *
 * @details
 * Realiza o agrupamento de entidades (INSERT + TEXTs) compartilhando a mesma
 * coordenada UTM. Suporta separadores \r, \n e \x1a (EOF Leica).
 *
 * @format_pnt Estrutura da Linha (Fixo: 68 bytes + \n):
 * - ID do Ponto: 16 chars | Atributo: 16 chars | N, E, Z: 12 chars cada.
 */

#ifndef DXFPARSER_H
#define DXFPARSER_H

#include <QString>
#include <QVector>
#include <QFile>
#include <QRegularExpression>
#include <QTextStream>

struct TopoPoint
{
    QString id = "";
    QString attr = "";
    double n = 0.0;
    double e = 0.0;
    double z = 0.0;
};

class DxfParser
{
public:
    // Carrega o arquivo DXF e retorna o vetor de pontos agrupados
    static QVector<TopoPoint> loadAndParse(const QString &filePath);

    // Salva a lista de pontos no formato de comprimento fixo .pnt
    static bool saveToPnt(const QString &filePath, const QVector<TopoPoint> &points);

    // Formata um único objeto TopoPoint para a string de 68 caracteres
    static QString formatToFixedLine(const TopoPoint &p);

    static bool fazCadScr(const QString &filePath, const QVector<TopoPoint> &points);

private:
    // Lógica interna de processamento de blocos DXF
    static QVector<TopoPoint> parseLeicaDxf(const QByteArray &data);
};

#endif // DXFPARSER_H
