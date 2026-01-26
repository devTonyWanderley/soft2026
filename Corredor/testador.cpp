#include "testador.h"
#include "ui_testador.h"
#include "../Banco/importengine.h"

void rodarTesteArquitetura() {
    qDebug() << "--- INICIANDO TESTE INTEGRADO: SEÇÕES TRANSVERSAIS ---";
    GerenciadorDeArquivos gerenciador;
    Corredor corredor;

    // 1. Carga da Malha (Pdw + Ars + Faces + Contorno)
    // ... (Use as mesmas configurações de layout pPdw que validamos antes)
    ParametrosLayout pPdw; // ... (configuração 16/12 caracteres e escala 10000)
    pPdw.config.larguraFixa = true;
    pPdw.config.fatorEscala = 10000.0; // Décimos de milímetro
    // Nomes devem ser EXATAMENTE os que o ImportEngine usa
    pPdw.config.mapaColunas["ID"]  = {0, 16};
    pPdw.config.mapaColunas["ATR"] = {16, 16};
    pPdw.config.mapaColunas["Y"]   = {32, 12};
    pPdw.config.mapaColunas["X"]   = {44, 12};
    pPdw.config.mapaColunas["Z"]   = {56, 12};

    ImportadorTexto proc;
    gerenciador.adicionarAFila("C:/2026/Soft/Instâncias/Pontos.pdw", Camada::Primitiva, pPdw);
    gerenciador.executarBatch(&proc);
    gerenciador.vincularArestasARS(Camada::Primitiva, "C:/2026/Soft/Instâncias/Arestas.ars");
    gerenciador.reconstruirFaces(Camada::Primitiva);

    Superficie* terreno = gerenciador.obterSuperficie(Camada::Primitiva);
    terreno->gerarContornoSequencial(); // Importante para o futuro limite

    // 2. Carga do Eixo e Perfil
    /*

    ParametrosLayout pEixo; // ... (configuração 15/15/12)
    pEixo.config.larguraFixa = true;
    // Certifique-se de que estas chaves são idênticas às usadas no .cpp
    pEixo.config.mapaColunas["X"]     = {0, 15};
    pEixo.config.mapaColunas["Y"]     = {15, 15};
    pEixo.config.mapaColunas["BULGE"] = {30, 12};

    auto dadosEixo = ImportEngine::importarEixo("C:/2026/Soft/Instâncias/tracado.txt", pEixo.config);
    corredor.horizontal = EixoHorizontal(dadosEixo, 0.0);

    */

    // ... (Use a configuração pEixo 15/15/12)
    ParametrosLayout pEixo; // ... (configuração 15/15/12)
    pEixo.config.larguraFixa = true;
    // Certifique-se de que estas chaves são idênticas às usadas no .cpp
    pEixo.config.mapaColunas["X"]     = {0, 15};
    pEixo.config.mapaColunas["Y"]     = {15, 15};
    pEixo.config.mapaColunas["BULGE"] = {30, 12};
    auto dadosEixo = ImportEngine::importarEixo("C:/2026/Soft/Instâncias/tracado.txt", pEixo.config);
    corredor.horizontal = EixoHorizontal(dadosEixo, 0.0);

    qDebug() << "Consolidando Perfil Longitudinal...";
    corredor.gerarPerfilLongitudinal(*terreno);
    corredor.consolidarEstaqueamentoLongitudinal();

    // 3. GERAÇÃO DAS SEÇÕES TRANSVERSAIS
    qDebug() << "Gerando Amostragem TIN para Seções (Largura de busca: 50m)...";
    // Usamos 50m para garantir que pegamos tudo até o contorno
    corredor.gerarAmostragemTIN(*terreno, 50.0);

    // 4. EXPORTAÇÃO PARAMETRIZADA
    qDebug() << "Exportando Seções (Décimos de Milímetro)...";

    std::vector<std::map<QString, QString>> dadosExport;
    for (const auto& sec : corredor.secoes) {
        for (const auto& pt : sec.terreno) {
            std::map<QString, QString> linha;
            linha["ESTACA"] = ExportEngine::formatarValor(sec.estaca, 12);
            linha["TEXTO1"] = ExportEngine::formatarTexto("OFFSET", 16);
            linha["OFFSET"] = ExportEngine::formatarValor(pt.offset, 12);
            linha["TEXTO2"] = ExportEngine::formatarTexto("COTA_Z", 16);
            linha["COTA"]   = ExportEngine::formatarValor(pt.cota, 12);
            dadosExport.push_back(linha);
        }
    }

    std::vector<ColunaExport> layout = {
        {"ESTACA", 12, true}, {"TEXTO1", 16, false}, {"OFFSET", 12, true},
        {"TEXTO2", 16, false}, {"COTA", 12, true}
    };

    ExportEngine::salvarFixo("C:/2026/Soft/Instâncias/secoes_primitiva.txt", dadosExport, layout);

    qDebug() << "========================================";
    qDebug() << "TESTE CONCLUÍDO:";
    qDebug() << "Total de Seções Geradas:" << corredor.secoes.size();
    if(!corredor.secoes.empty())
        qDebug() << "Pontos na Estaca 0:" << corredor.secoes[0].terreno.size();
    qDebug() << "========================================";
}


testador::testador(QWidget *parent): QMainWindow(parent), ui(new Ui::testador)
{
    ui->setupUi(this);
    rodarTesteArquitetura();
}

testador::~testador()
{
    delete ui;
}
