#include "StorageProvider.h"
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QFileInfo>
#include <QRegularExpression>
#include <cmath>

// Implementação da leitura de pontos (PDW)
std::vector<Ponto> StorageProvider::lerPontos(const QString& path, const FileLayout& layout) {
    std::vector<Ponto> lista;
    // Usa QDir::toNativeSeparators para garantir portabilidade do caminho
    QString caminhoFormatado = QDir::cleanPath(path);
    QFile arquivo(caminhoFormatado);

    if (!arquivo.open(QIODevice::ReadOnly | QIODevice::Text)) return lista;

    QTextStream in(&arquivo);
    while (!in.atEnd()) {
        QString linha = in.readLine();
        if (linha.trimmed().isEmpty()) continue;

        auto obterCampo = [&](const QString& nome) {
            if (!layout.colunas.count(nome)) return QString("");
            auto pos = layout.colunas.at(nome);
            return linha.mid(pos.first, pos.second).trimmed();
        };

        double x = obterCampo("X").toDouble() / layout.escala;
        double y = obterCampo("Y").toDouble() / layout.escala;
        double z = obterCampo("Z").toDouble() / layout.escala;

        lista.emplace_back(obterCampo("ID"), obterCampo("ATR"), x, y, z);
    }
    return lista;
}

struct TempVertice
{
    Ponto p;
    double b;
};

// Implementação da leitura do Eixo (Lisp -> C++)
std::vector<DadosBrutos> StorageProvider::lerEixo(const QString& path, const FileLayout& layout) {
    std::vector<DadosBrutos> segmentos;
    std::vector<TempVertice> tempVerts;

    QFile arquivo(QDir::cleanPath(path));
    if (!arquivo.open(QIODevice::ReadOnly | QIODevice::Text)) return segmentos;

    QTextStream in(&arquivo);
    while (!in.atEnd()) {
        QString linha = in.readLine();
        if (linha.length() < 30) continue;

        double x = linha.mid(layout.colunas.at("X").first, layout.colunas.at("X").second).toDouble();
        double y = linha.mid(layout.colunas.at("Y").first, layout.colunas.at("Y").second).toDouble();
        double b = linha.mid(layout.colunas.at("BULGE").first, layout.colunas.at("BULGE").second).toDouble();

        tempVerts.push_back({ Ponto("", "", x, y, 0.0), b });
    }

    for (size_t i = 0; i < tempVerts.size() - 1; ++i) {
        segmentos.emplace_back(tempVerts[i].p, tempVerts[i+1].p, tempVerts[i].b);
    }
    return segmentos;
}

// Implementação da vinculação de Arestas (ARS)
void StorageProvider::lerArestas(const QString& path, Superficie& superf) {
    QFile arquivo(QDir::cleanPath(path));
    if (!arquivo.open(QIODevice::ReadOnly | QIODevice::Text)) return;

    // Mapa temporário para tradução ID -> Índice (Estratégia "Carrega e Esquece")
    std::map<QString, int> mapaIds;
    for (int i = 0; i < (int)superf.pontos.size(); ++i) {
        mapaIds[superf.pontos[i].nome] = i;
    }

    QTextStream in(&arquivo);
    while (!in.atEnd()) {
        QString linha = in.readLine().trimmed();
        QStringList ids = linha.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);

        if (ids.size() >= 2) {
            if (mapaIds.count(ids[0]) && mapaIds.count(ids[1])) {
                superf.arestas.emplace_back(mapaIds[ids[0]], mapaIds[ids[1]]);
            }
        }
    }
}

// Exportador Parametrizado (Décimos de Milímetro)
bool StorageProvider::exportarFixo(const QString& path,
                                   const std::vector<std::map<QString, QString>>& dados,
                                   const std::vector<std::pair<QString, int>>& colunas) {
    QFile arquivo(QDir::cleanPath(path));
    if (!arquivo.open(QIODevice::WriteOnly | QIODevice::Text)) return false;

    QTextStream out(&arquivo);
    for (const auto& linha : dados) {
        QString linhaFinal = "";
        for (const auto& col : colunas) {
            QString valor = linha.at(col.first);
            // Preenchimento de espaços à direita para texto, ou truncamento se maior que a coluna
            linhaFinal += valor.leftJustified(col.second, ' ').left(col.second);
        }
        out << linhaFinal << "\n";
    }
    return true;
}

QString StorageProvider::formatarValor(double val, int largura) {
    // Multiplica por 10.000 para décimo de milímetro e arredonda
    long long valorInt = static_cast<long long>(std::round(val * 10000.0));
    // Formata com zeros à esquerda
    return QString("%1").arg(valorInt, largura, 10, QChar('0')).right(largura);
}

QString StorageProvider::formatarTexto(QString txt, int largura) {
    // Justifica à esquerda com espaços
    return txt.leftJustified(largura, ' ').left(largura);
}
