#include "../BancoDxf/dxfparse.h"
#include <QtMath>
#include <QLocale>

QVector<TopoPoint> DxfParser::loadAndParse(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) return QVector<TopoPoint>();

    QByteArray data = file.readAll();
    file.close();
    return parseLeicaDxf(data);
}

QVector<TopoPoint> DxfParser::parseLeicaDxf(const QByteArray &data)
{
    QVector<TopoPoint> pontos;
    QString conteudo = QString::fromLatin1(data);

    // Regex suportando \r, \n e o caractere EOF \x1a
    QStringList lt = conteudo.split(QRegularExpression("[\r\n\x1a]+"), Qt::SkipEmptyParts);


    TopoPoint pontoAtual;
    QString layerAtual;
    double lastX = -999999.0; // Valor sentinela para primeira detecção
    bool temDados = false;
    bool dentroEntities = false; // A bandeira de controle

    for (int i = 0; i < lt.size() - 1; i += 2)
    {
        QString rotulo = lt.at(i).trimmed();
        QString valor  = lt.at(i + 1).trimmed();

        // Controle de Seção
        if (rotulo == "0" && valor == "SECTION") continue;
        if (valor == "ENTITIES")
        {
            dentroEntities = true;
            continue;
        }
        if (valor == "ENDSEC" && dentroEntities)
        {
            dentroEntities = false;
            break;
        }

        // Só processa se estiver dentro da área correta
        if(!dentroEntities) continue;

        if (rotulo == "8") {
            layerAtual = valor.toUpper();
        }
        else if (rotulo == "10")
        {
            double x = valor.toDouble();
            // Se mudou de coordenada X, entende-se como novo ponto físico
            if (qAbs(x - lastX) > 0.0001)
            {
                if (temDados&& !pontoAtual.id.isEmpty()) pontos.append(pontoAtual);
                pontoAtual = TopoPoint();
                pontoAtual.e = x;
                lastX = x;
                temDados = true;
            }
        }
        else if (rotulo == "20") pontoAtual.n = valor.toDouble();
        else if (rotulo == "30") pontoAtual.z = valor.toDouble();
        else if (rotulo == "1")
        {
            if (layerAtual == "PT_ID") pontoAtual.id = valor;
            else if (layerAtual == "CODE") pontoAtual.attr = valor;
        }
    }
    if (temDados) pontos.append(pontoAtual);
    return pontos;
}

bool DxfParser::saveToPnt(const QString &filePath, const QVector<TopoPoint> &points)
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) return false;

    QTextStream out(&file);
    // Garante compatibilidade de 1 byte por caractere para a máscara fixa
    out.setEncoding(QStringConverter::Latin1);

    for (const TopoPoint &p : points)
    {
        out << formatToFixedLine(p) << "\n";
    }
    file.close();
    return true;
}

QString DxfParser::formatToFixedLine(const TopoPoint &p)
{
    // Escala para décimos de milímetro (0.1 mm)
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

bool DxfParser::fazCadScr(const QString &filePath, const QVector<TopoPoint> &points)
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) return false;

    QTextStream out(&file);
    // Força o ponto como separador decimal (essencial para o CAD)
    out.setLocale(QLocale::C);

    // 1. Cabeçalho do Script (LISP)
    out << ";; Script gerado automaticamente pelo parse\n";
    out << "(if (not (tblsearch \"APPID\" \"WPNT\")) (regapp \"WPNT\"))\n";

    // 2. Loop de geração das entidades POINT com XData
    for (const TopoPoint &p : points) {
        // Formato: (entmake (list '(0 . "POINT") (list 10 X Y Z) (list -3 (list "APP" '(1000 . "ID") '(1000 . "ATTR")))))
        out << "(entmake (list "
            << "'(0 . \"POINT\") "
            << "(list 10 "
            << QString::number(p.e, 'f', 4) << " "
            << QString::number(p.n, 'f', 4) << " "
            << QString::number(p.z, 'f', 4) << ") "
            << "(list -3 (list \"WPNT\" "
            << "'(1000 . \"" << p.id.trimmed() << "\") "
            << "'(1000 . \"" << p.attr.trimmed() << "\")))))\n";
    }

    // 3. Finalização
    out << "_ZOOM _E\n";
    file.close();
    return true;
}
