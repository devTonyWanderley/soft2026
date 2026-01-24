#ifndef IMPORTENGINE_H
#define IMPORTENGINE_H

#include "revTipos.h" // Onde está seu struct Ponto e enum Camada
#include <QString>
#include <vector>
#include <map>

/*
   - [x] Importação de Pontos (.pdw)
   - [ ] Importação de Arestas (.ars) -> FOCO ATUAL
*/

// Configuração para extração de dados
struct LayoutConfig
{
    bool larguraFixa = true;
    char delimitador = ';';
    double fatorEscala = 1.0; // Valor padrão (sem alteração)
    // Mapeamento: "NomeCampo" -> {Início, Comprimento}
    std::map<QString, std::pair<int, int>> mapaColunas;
};

struct ParametrosLayout
{
    LayoutConfig config; // Contém larguraFixa, fatorEscala, mapaColunas, etc.
};

class IProvedorPontos {
public:
    virtual ~IProvedorPontos() = default;
    virtual std::vector<Ponto> processar(const QString& caminho, const ParametrosLayout& params) = 0;
};

class ImportEngine
{
public:
    ImportEngine();

    // Rotina Paramétrica Principal
    static std::vector<Ponto> processarArquivo(const QString& caminho, const LayoutConfig& config);

    // Utilitário para validar se a linha é válida antes do parse
    static bool linhaValida(const QString& linha);

    static std::vector<DadosBrutos> importarEixo(const QString& caminho, const LayoutConfig& config);

private:
    // Sub-rotinas internas para evitar repetição de código
    static Ponto extrairLarguraFixa(const QString& linha, const LayoutConfig& config);
    static Ponto extrairDelimitado(const QString& linha, const LayoutConfig& config);
};

// Agora o ImportEngine "Assina o Contrato" da Interface
class ImportadorTexto : public IProvedorPontos
{
public:
    std::vector<Ponto> processar(const QString& caminho, const ParametrosLayout& params) override
    {
        // Chamamos a rotina paramétrica que você já validou no teste
        return ImportEngine::processarArquivo(caminho, params.config);
    }
};

class GerenciadorDeArquivos {
    struct Tarefa {
        QString caminho;
        Camada camada;
        ParametrosLayout params;
    };
    std::vector<Tarefa> fila;
    std::map<Camada, Superficie> repositorio;

public:
    void adicionarAFila(QString p, Camada c, ParametrosLayout par) {
        fila.push_back({p, c, par});
    }

    // APENAS A DECLARAÇÃO AQUI
    void executarBatch(IProvedorPontos* processador);

    // ADICIONE ESTA DEFINIÇÃO AQUI:
    Superficie* obterSuperficie(Camada tipo) {
        if (repositorio.count(tipo)) {
            return &repositorio[tipo];
        }
        return nullptr;
    }
    void vincularArestasARS(Camada tipo, const QString& caminhoArs);
    void reconstruirFaces(Camada tipo);
};

#endif // IMPORTENGINE_H
