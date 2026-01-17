#include "dxfparser.h"
#include <QtMath>

QVector<TopoPoint> dxfparser::parseLeicaDxf(const QByteArray &data) {
    QVector<TopoPoint> pontos;
    // Converte e limpa usando o Regex com o EOF \x1a incluso
    QString conteudo = QString::fromLatin1(data);
    QStringList lt = conteudo.split(QRegularExpression("[\r\n\x1a]+"), Qt::SkipEmptyParts);

    TopoPoint pontoAtual;
    bool temDados = false;

    for (int i = 0; i < lt.size() - 1; i += 2) {
        QString rotulo = lt.at(i).trimmed();
        QString valor  = lt.at(i + 1).trimmed();

        if (rotulo == "10") { pontoAtual.e = valor.toDouble(); temDados = true; }
        else if (rotulo == "20") { pontoAtual.n = valor.toDouble(); }
        else if (rotulo == "30") { pontoAtual.z = valor.toDouble(); }
        else if (rotulo == "1") {
            if (pontoAtual.id.isEmpty()) pontoAtual.id = valor;
            else pontoAtual.attr = valor;
        }
        // No DXF, o código '0' indica início de nova entidade (ex: 0 e depois INSERT ou TEXT)
        else if (rotulo == "0" && temDados) {
            pontos.append(pontoAtual);
            pontoAtual = TopoPoint(); // Reseta
            temDados = false;
        }
    }
    return pontos;
}

QString dxfparser::formatToFixedLine(const TopoPoint &p) {
    // Escala para décimos de milímetro (10.000)
    long long nDec = qRound64(p.n * 10000);
    long long eDec = qRound64(p.e * 10000);
    long long zDec = qRound64(p.z * 10000);

    return QString("%1%2%3%4%5")
        .arg(p.id.leftJustified(16, ' '))
        .arg(p.attr.leftJustified(16, ' '))
        .arg(QString::number(nDec).rightJustified(12, '0'))
        .arg(QString::number(eDec).rightJustified(12, '0'))
        .arg(QString::number(zDec).rightJustified(12, '0'));
}
