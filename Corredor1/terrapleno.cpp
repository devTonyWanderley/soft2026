#include "terrapleno.h"
#include "ui_terrapleno.h"

Terrapleno::Terrapleno(QWidget *parent): QMainWindow(parent), ui(new Ui::Terrapleno)
{
    ui->setupUi(this);
}

void Terrapleno::on_btnProcessar_clicked()
{
    // 1. Definições de Caminho e Nome do Trabalho
    QString dirTrabalho = "C:/2026/Soft/Instâncias/";
    QString raizNome = "Projeto_01"; // Futuramente você pegará isso de um QLineEdit

    // 2. Configuração de Layouts
    FileLayout pPdw;
    pPdw.escala = 10000.0;
    pPdw.colunas["ID"] = {0, 16}; pPdw.colunas["ATR"] = {16, 16};
    pPdw.colunas["Y"] = {32, 12}; pPdw.colunas["X"] = {44, 12}; pPdw.colunas["Z"] = {56, 12};

    FileLayout pEixo;
    pEixo.colunas["X"] = {0, 15}; pEixo.colunas["Y"] = {15, 15}; pEixo.colunas["BULGE"] = {30, 12};

    ui->statusbar->showMessage("Processando: " + raizNome);

    // 3. Execução do Motor (Leitura e Malha)
    m_primitiva.pontos = StorageProvider::lerPontos(dirTrabalho + "Pontos.pdw", pPdw);
    StorageProvider::lerArestas(dirTrabalho + "Arestas.ars", m_primitiva);
    m_primitiva.gerarContornoSequencial();

    // 4. Eixo e Perfil
    auto dadosEixo = StorageProvider::lerEixo(dirTrabalho + "tracado.txt", pEixo);
    m_corredor.horizontal = EixoHorizontal(dadosEixo, 0.0);
    m_corredor.gerarPerfilLongitudinal(m_primitiva);
    m_corredor.consolidarEstaqueamentoLongitudinal();

    // 5. Seções
    m_corredor.gerarAmostragemTIN(m_primitiva, 50.0);

    // 6. Exportação Parametrizada via StorageProvider (ou seu Exportador)
    // --- Exportar Perfil ---
    std::vector<std::map<QString, QString>> dadosPerfil;
    for (const auto& p : m_corredor.vertical.pontos) {
        std::map<QString, QString> linha;
        linha["EST"] = StorageProvider::formatarValor(p.estaca, 12);
        linha["TXT"] = StorageProvider::formatarTexto("COTA_Z", 16);
        linha["Z"]   = StorageProvider::formatarValor(p.cota, 12);
        dadosPerfil.push_back(linha);
    }
    std::vector<std::pair<QString, int>> layPerfil = {{"EST", 12}, {"TXT", 16}, {"Z", 12}};
    StorageProvider::exportarFixo(dirTrabalho + raizNome + "_perfil.txt", dadosPerfil, layPerfil);

    // --- Exportar Seções ---
    std::vector<std::map<QString, QString>> dadosSec;
    for (const auto& sec : m_corredor.secoes) {
        for (const auto& pt : sec.terreno) {
            std::map<QString, QString> linha;
            linha["EST"] = StorageProvider::formatarValor(sec.estaca, 12);
            linha["TX1"] = StorageProvider::formatarTexto("OFF", 16);
            linha["OFF"] = StorageProvider::formatarValor(pt.offset, 12);
            linha["TX2"] = StorageProvider::formatarTexto("Z", 16);
            linha["Z"]   = StorageProvider::formatarValor(pt.cota, 12);
            dadosSec.push_back(linha);
        }
    }
    std::vector<std::pair<QString, int>> laySec = {{"EST", 12}, {"TX1", 16}, {"OFF", 12}, {"TX2", 16}, {"Z", 12}};
    StorageProvider::exportarFixo(dirTrabalho + raizNome + "_secoes.txt", dadosSec, laySec);

    ui->statusbar->showMessage("Arquivos gerados em: " + dirTrabalho);
}

Terrapleno::~Terrapleno()
{
    delete ui;
}
