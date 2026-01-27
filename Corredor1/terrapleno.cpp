#include "terrapleno.h"
#include "ui_terrapleno.h"

Terrapleno::Terrapleno(QWidget *parent): QMainWindow(parent), ui(new Ui::Terrapleno)
{
    ui->setupUi(this);
}

void Terrapleno::on_btnProcessar_clicked() {
    // 1. Identificação e Caminhos (Tony's Workstation)
    QString idProjeto = ui->leIdProjeto->text().trimmed();
    if (idProjeto.isEmpty()) idProjeto = "NOVA_SEDE_F1";

    QString dir = "C:/2026/NovaSede/";

    ui->statusbar->showMessage("🚀 Iniciando Motor de Terraplenagem...");

    // 2. CONFIGURAÇÃO DE LAYOUTS (Padrão validado de 12 e 16 colunas)
    FileLayout pPdw;
    pPdw.escala = 10000.0;
    pPdw.colunas["ID"]={0,16}; pPdw.colunas["Y"]={32,12}; pPdw.colunas["X"]={44,12}; pPdw.colunas["Z"]={56,12};

    FileLayout pEixo;
    pEixo.colunas["X"]={0,15}; pEixo.colunas["Y"]={15,15}; pEixo.colunas["BULGE"]={30,12};

    // 3. CARGA E TOPOLOGIA (O Alicerce)
    m_primitiva.pontos = StorageProvider::lerPontos(dir + "Pontos.pdw", pPdw);
    StorageProvider::lerArestas(dir + "Arestas.ars", m_primitiva);

    // Gera o contorno de 101 pontos para o Raycasting das seções
    m_primitiva.gerarContornoSequencial();

    // 4. EIXO E PERFIL (O Trilho)
    auto dadosEixo = StorageProvider::lerEixo(dir + "tracado.txt", pEixo);
    if(dadosEixo.empty()) {
        ui->statusbar->showMessage("❌ ERRO: Arquivo tracados.txt não encontrado!");
        return;
    }
    m_corredor.horizontal = EixoHorizontal(dadosEixo, 0.0);

    // 5. PROCESSAMENTO E EXPORTAÇÃO (A Entrega)
    // Gera o perfil longitudinal fiel e as estacas de projeto (PC, PT, INT)
    m_corredor.gerarPerfilEConsolidar(m_primitiva);
    // --- ADICIONE ESTA LINHA AQUI ---
    m_corredor.exportarPerfilIdentificado(dir + idProjeto + "_perfil.txt");

    // Gera e salva os arquivos individuais de seção (com o cabeçalho SEC_ESTACA)
    ui->statusbar->showMessage("💾 Gravando seções individuais...");
    m_corredor.gerarArquivosSecoes(m_primitiva, dir, idProjeto);

    ui->statusbar->showMessage("✅ Sucesso! Perfil e Seções gerados em: " + dir);
}



Terrapleno::~Terrapleno()
{
    delete ui;
}
