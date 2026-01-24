#include "ImportEngine.h"
#include <QFile>
#include <QTextStream>
#include <set>
#include <map>
#include <vector>
#include <algorithm>
#include <QRegularExpression>

std::vector<Ponto> ImportEngine::processarArquivo(const QString& caminho, const LayoutConfig& config)
{
    std::vector<Ponto> listaResultante;
    QFile arquivo(caminho);

    if (!arquivo.open(QIODevice::ReadOnly | QIODevice::Text)) return listaResultante;

    QTextStream in(&arquivo);
    while (!in.atEnd())
    {
        QString linha = in.readLine().trimmed();

        if (!linhaValida(linha)) continue;

        if (config.larguraFixa)
        {
            listaResultante.push_back(extrairLarguraFixa(linha, config));
        }
        else
        {
            listaResultante.push_back(extrairDelimitado(linha, config));
        }
    }
    return listaResultante;
}

bool ImportEngine::linhaValida(const QString& linha)
{
    if (linha.isEmpty() || linha.startsWith("#") || linha.startsWith("//"))
        return false;
    return true;
}

Ponto ImportEngine::extrairLarguraFixa(const QString& linha, const LayoutConfig& config)
{
    auto obterCampo = [&](const QString& nome)
    {
        auto pos = config.mapaColunas.at(nome);
        return linha.mid(pos.first, pos.second).trimmed();
    };

    // Aplicamos a divisão pelo fator (ex: 10000.0) diretamente aqui
    double x = obterCampo("X").toDouble() / config.fatorEscala;
    double y = obterCampo("Y").toDouble() / config.fatorEscala;
    double z = obterCampo("Z").toDouble() / config.fatorEscala;

    return Ponto(obterCampo("ID"), obterCampo("ATR"), x, y, z);
}

// Implementação futura para CSV/TXT delimitado
Ponto ImportEngine::extrairDelimitado(const QString& linha, const LayoutConfig& config)
{
    QStringList campos = linha.split(config.delimitador);
    // Lógica similar usando os índices...
    return Ponto();
}

std::vector<DadosBrutos> ImportEngine::importarEixo(const QString& caminho, const LayoutConfig& config)
{
    std::vector<DadosBrutos> listaEixo;

    // 1. Primeiro lemos todos os vértices como pontos temporários
    std::vector<Ponto> vertices;
    QFile arquivo(caminho);
    if (!arquivo.open(QIODevice::ReadOnly | QIODevice::Text)) return listaEixo;

    QTextStream in(&arquivo);
    struct TempVert
    {
        Ponto p;
        double b;
    };
    std::vector<TempVert> temp;

    while (!in.atEnd())
    {
        QString linha = in.readLine();
        if (linha.length() < 30) continue;

        double x = linha.mid(config.mapaColunas.at("X").first, config.mapaColunas.at("X").second).toDouble();
        double y = linha.mid(config.mapaColunas.at("Y").first, config.mapaColunas.at("Y").second).toDouble();
        double b = linha.mid(config.mapaColunas.at("BULGE").first, config.mapaColunas.at("BULGE").second).toDouble();

        temp.push_back({Ponto("", "", x, y, 0.0), b});
    }

    // 2. Montamos os segmentos: o segmento i vai do ponto i ao i+1 com o bulge de i
    for (size_t i = 0; i < temp.size() - 1; ++i)
    {
        listaEixo.emplace_back(temp[i].p, temp[i+1].p, temp[i].b);
    }

    return listaEixo;
}



// AGORA SIM: Implemente a função usando o escopo ::
void GerenciadorDeArquivos::executarBatch(IProvedorPontos* processador)
{
    for (const auto& t : fila)
    {
        std::vector<Ponto> pts = processador->processar(t.caminho, t.params);
        repositorio[t.camada].pontos = std::move(pts);
    }
    fila.clear();
}

void GerenciadorDeArquivos::vincularArestasARS(Camada tipo, const QString& caminhoArs)
{
    Superficie* s = obterSuperficie(tipo);
    if (!s) return;

    // 1. Criar o Mapa de Tradução Temporário
    // Chave: Nome do Ponto (ID no ARS) -> Valor: Posição no vetor (Índice)
    std::map<QString, int> dicionarioIds;
    for (int i = 0; i < (int)s->pontos.size(); ++i)
    {
        dicionarioIds[s->pontos[i].nome] = i;
    }

    // 2. Abrir o arquivo .ars
    QFile arquivo(caminhoArs);
    if (!arquivo.open(QIODevice::ReadOnly | QIODevice::Text)) return;

    QTextStream in(&arquivo);
    while (!in.atEnd())
    {
        QString linha = in.readLine();
        // Exemplo de formato: "329             330" (ajustado para largura fixa)
        QString idIni = linha.mid(0, 16).trimmed();
        QString idFim = linha.mid(16, 16).trimmed();

        // 3. Tradução Imediata: O ID entra, o Índice sai
        if (dicionarioIds.count(idIni) && dicionarioIds.count(idFim))
        {
            // Criamos a aresta já com os índices 0, 1, 2...
            s->arestas.emplace_back(dicionarioIds[idIni], dicionarioIds[idFim]);
        }
    }
}

void GerenciadorDeArquivos::reconstruirFaces(Camada tipo)
{
    Superficie* s = obterSuperficie(tipo);
    if (!s || s->arestas.empty()) return;

    // 1. Criar lista de adjacência (Quais pontos estão ligados a quem?)
    // Chave: Índice do Ponto -> Valor: Lista de vizinhos
    std::map<int, std::set<int>> adj;
    for (const auto& a : s->arestas)
    {
        adj[a.iIni].insert(a.iFim);
        adj[a.iFim].insert(a.iIni);
    }

    s->faces.clear();
    std::set<std::vector<int>> facesDetectadas; // Para evitar duplicatas

    // 2. Para cada aresta (A-B), procuramos um vizinho comum 'C'
    for (const auto& a : s->arestas)
    {
        int u = a.iIni;
        int v = a.iFim;

        // Vizinhos de 'u'
        for (int vizinho : adj[u]) {
            // Se 'vizinho' também é vizinho de 'v', temos um triângulo!
            if (vizinho != v && adj[v].count(vizinho))
            {
                // Ordenar os índices para garantir que {1,2,3} seja igual a {3,2,1}
                std::vector<int> tri = {u, v, vizinho};
                std::sort(tri.begin(), tri.end());

                if (facesDetectadas.find(tri) == facesDetectadas.end())
                {
                    s->faces.emplace_back(tri[0], tri[1], tri[2]);
                    facesDetectadas.insert(tri);
                }
            }
        }
    }
    qDebug() << "Malha reconstruída:" << s->faces.size() << "faces (triângulos).";
}
