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

    // 1. Criar o Mapa de IDs (Trimmed para segurança)
    // Converte o nome do ponto (string) no índice (inteiro) do vetor
    std::map<QString, int> mapaIds;
    for (int i = 0; i < (int)superf.pontos.size(); ++i) {
        mapaIds[superf.pontos[i].nome.trimmed()] = i;
    }

    QTextStream in(&arquivo);
    int contagemSucesso = 0;
    while (!in.atEnd()) {
        QString linha = in.readLine().trimmed();
        if (linha.isEmpty()) continue;

        // Suporta espaços, tabulações ou múltiplos espaços
        QStringList ids = linha.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);

        if (ids.size() >= 2) {
            QString id1 = ids[0].trimmed();
            QString id2 = ids[1].trimmed();

            if (mapaIds.count(id1) && mapaIds.count(id2)) {
                superf.arestas.emplace_back(mapaIds[id1], mapaIds[id2]);
                contagemSucesso++;
            }
        }
    }
    arquivo.close();
    // Use o qDebug para validar se a malha foi montada
    qDebug() << "Malha montada com" << contagemSucesso << "arestas vinculadas.";
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

double StorageProvider::calcularDistanciaAoContorno(const Eigen::Vector2d& origem,
                                                    const Eigen::Vector2d& direcao,
                                                    const Superficie& terreno) {
    double menorT = 1e15; // "Infinito"
    bool atingiu = false;

    // Se o contorno não foi gerado, retorna um valor padrão (ex: 50m) para não travar
    if (terreno.indicesContorno.size() < 3) return 50.0;

    for (size_t i = 0; i < terreno.indicesContorno.size(); ++i) {
        // Pega os pontos do segmento de contorno (P1 -> P2)
        int idx1 = terreno.indicesContorno[i];
        int idx2 = terreno.indicesContorno[(i + 1) % terreno.indicesContorno.size()];

        Eigen::Vector2d p1 = terreno.pontos[idx1].pos2d();
        Eigen::Vector2d p2 = terreno.pontos[idx2].pos2d();
        Eigen::Vector2d vCont = p2 - p1;

        // Cramer: origem + t*direcao = p1 + u*vCont
        // Det = (dir.x * -vCont.y) - (dir.y * -vCont.x)
        double det = -direcao.x() * vCont.y() + direcao.y() * vCont.x();

        if (std::abs(det) < 1e-9) continue; // Paralelos

        double t = (vCont.x() * (origem.y() - p1.y()) - vCont.y() * (origem.x() - p1.x())) / det;
        double u = (direcao.x() * (origem.y() - p1.y()) - direcao.y() * (origem.x() - p1.x())) / det;

        // Se t > 0 (frente do raio) e 0 <= u <= 1 (dentro do segmento da borda)
        if (t > 0.001 && u >= 0.0 && u <= 1.0) {
            if (t < menorT) {
                menorT = t;
                atingiu = true;
            }
        }
    }

    // Se não atingiu o contorno (ponto fora?), retorna 50m como segurança
    return atingiu ? menorT : 50.0;
}

bool StorageProvider::exportarSecaoIndividual(const QString& path, double estaca, const std::vector<PontoSecao>& pontos) {
    std::vector<std::map<QString, QString>> dados;

    // Cabeçalho da Seção (Primeira Linha)
    std::map<QString, QString> cabecalho;
    cabecalho["C1"] = formatarTexto("SEC_ESTACA", 16);
    cabecalho["C2"] = formatarValor(estaca, 12);
    dados.push_back(cabecalho);

    // Corpo da Seção (Pontos Offset/Cota)
    for (const auto& pt : pontos) {
        std::map<QString, QString> linha;
        linha["C1"] = formatarValor(pt.offset, 16);
        linha["C2"] = formatarValor(pt.cota, 12);
        dados.push_back(linha);
    }

    return exportarFixo(path, dados, {{"C1", 16}, {"C2", 12}});
}
