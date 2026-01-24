#include "testador.h"
#include "ui_testador.h"
#include "../Banco/importengine.h"

void rodarTesteArquitetura() {
    qDebug() << "--- Iniciando Teste de Importação Completa (Pontos + Arestas) ---";

    // 1. Instanciar o Gerenciador (O Repositório)
    GerenciadorDeArquivos gerenciador;

    // 2. Configurar o Layout do PDW (Exatamente como validamos no mercado)
    ParametrosLayout paramsPDW;
    paramsPDW.config.larguraFixa = true;
    paramsPDW.config.fatorEscala = 10000.0;
    paramsPDW.config.mapaColunas["ID"]  = {0, 16};
    paramsPDW.config.mapaColunas["ATR"] = {16, 16};
    paramsPDW.config.mapaColunas["Y"]   = {32, 12};
    paramsPDW.config.mapaColunas["X"]   = {44, 12};
    paramsPDW.config.mapaColunas["Z"]   = {56, 12};

    // 3. Importar Pontos (A base da pirâmide)
    // Usamos o ImportadorTexto que criamos com a Interface IProvedorPontos
    ImportadorTexto processador;

    qDebug() << "Passo 1: Carregando pontos .pdw...";
    gerenciador.adicionarAFila("C:/2026/Soft/Instâncias/Pontos.pdw", Camada::Primitiva, paramsPDW);
    gerenciador.executarBatch(&processador);

    // 4. Importar e Vincular Arestas (A malha vinda do seu Lisp)
    qDebug() << "Passo 2: Vinculando arestas .ars...";
    gerenciador.vincularArestasARS(Camada::Primitiva, "C:/2026/Soft/Instâncias/Arestas.ars");

    // 4. RECONSTRUÇÃO DE FACES (O novo passo!)
    qDebug() << "Processando topologia para criar faces...";
    gerenciador.reconstruirFaces(Camada::Primitiva);

    Superficie* s = gerenciador.obterSuperficie(Camada::Primitiva);

    //  Espaço pra ler traçado:
    // Layout correspondente à rotina AutoLISP: (rtos x 2 4) e larguras 15, 15, 12
    ParametrosLayout paramsEixo;
    paramsEixo.config.larguraFixa = true;
    paramsEixo.config.fatorEscala = 1.0; // Já está em metros no Lisp

    // Mapeamento das colunas (Início, Comprimento)
    paramsEixo.config.mapaColunas["X"]     = {0, 15};
    paramsEixo.config.mapaColunas["Y"]     = {15, 15};
    paramsEixo.config.mapaColunas["BULGE"] = {30, 12};
    // ... seu código anterior ...

    // 1. Ler os dados brutos do arquivo gerado pelo Lisp
    QString caminhoEixo = "C:/2026/Soft/Instâncias/tracado.txt";
    std::vector<DadosBrutos> dadosEixo = ImportEngine::importarEixo(caminhoEixo, paramsEixo.config);

    // 2. Instanciar o Eixo (Isso dispara os cálculos de Raio, Delta e Estacas)
    if (!dadosEixo.empty()) {
        EixoHorizontal eixo(dadosEixo, 0.0); // Estaca zero inicial

        qDebug() << "========================================";
        qDebug() << "EIXO HORIZONTAL CARREGADO:";
        qDebug() << "Total de Segmentos:" << dadosEixo.size();

        // 3. Teste de Estaqueamento (Próximo passo lógico)
        // Vamos verificar o comprimento total do eixo
        // Note: você precisará de um método no EixoHorizontal para retornar a estaca final
        // qDebug() << "Extensão Total:" << eixo.getComprimentoTotal() << "m";
        qDebug() << "========================================";
    } else {
        qDebug() << "Erro: Não foi possível carregar os dados do eixo.";
    }


    // 5. Validação Final
    if (s) {
        qDebug() << "========================================";
        qDebug() << "RELATÓRIO DE SUPERFÍCIE:";
        qDebug() << "PONTOS  :" << s->pontos.size();
        qDebug() << "ARESTAS :" << s->arestas.size();
        qDebug() << "FACES   :" << s->faces.size() << " (Triângulos)";
        qDebug() << "========================================";

        if (!s->faces.empty()) {
            // Teste de cálculo vetorial com Eigen na primeira face
            const Face& f = s->faces[0];
            Eigen::Vector3d normal = f.calcularNormal(s->pontos);
            qDebug() << "Vetor Normal da Face 0:" << normal.x() << "," << normal.y() << "," << normal.z();

            // Verificação de inclinação (declividade)
            double declividade = std::acos(std::abs(normal.z())) * (180.0 / M_PI);
            qDebug() << "Inclinação da Face 0:" << declividade << "graus.";
        }
    }
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
