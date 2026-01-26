#include "terrapleno.h"
#include "ui_terrapleno.h"

Terrapleno::Terrapleno(QWidget *parent): QMainWindow(parent), ui(new Ui::Terrapleno)
{
    ui->setupUi(this);
}

void Terrapleno::on_btnProcessar_clicked()
{
    // 1. Definições de Caminho e Nome do Trabalho
    QString dirTrabalho = "C:/Tony/NovaSede/";
    QString raizNome = "Projeto_02"; // Futuramente você pegará isso de um QLineEdit

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
    auto dadosEixo = StorageProvider::lerEixo(dirTrabalho + "Tracado.txt", pEixo);
    m_corredor.horizontal = EixoHorizontal(dadosEixo, 0.0);
    m_corredor.gerarPerfilLongitudinal(m_primitiva);
    m_corredor.consolidarEstaqueamentoLongitudinal();



    // 5. Seções
    //m_corredor.gerarAmostragemTIN(m_primitiva, 50.0);

    // 5. Seções (Agora chamando a rotina de exportação individual)
    ui->statusbar->showMessage("Gerando seções individuais até o contorno...");

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
//........................................................
    // Substitua o loop de exportação de seções anterior por este:
    for (const auto& pPerfil : m_corredor.vertical.pontos) {
        SecaoTransversal secao(pPerfil.estaca);
        Eigen::Vector2d posEixo = m_corredor.horizontal.getXYNaEstaca(pPerfil.estaca);
        Eigen::Vector2d nPerp = m_corredor.horizontal.getPerpendicularNaEstaca(pPerfil.estaca);

        // A. Limites dinâmicos pelo Contorno (Raycasting)
        double dEsq = StorageProvider::calcularDistanciaAoContorno(posEixo, -nPerp, m_primitiva);
        double dDir = StorageProvider::calcularDistanciaAoContorno(posEixo, nPerp, m_primitiva);

        // B. Régua Transversal exata
        Ponto pEsq("", "", posEixo.x() - nPerp.x() * dEsq, posEixo.y() - nPerp.y() * dEsq);
        Ponto pDir("", "", posEixo.x() + nPerp.x() * dDir, posEixo.y() + nPerp.y() * dDir);

        // C. Interseção com a Malha (Cramer)
        for (const auto& aresta : m_primitiva.arestas) {
            auto inters = SegmentoHorizontal::interceptarRetaManual(pEsq, pDir, m_primitiva.pontos[aresta.iIni], m_primitiva.pontos[aresta.iFim]);
            for (const auto& pt : inters) {
                double off = (Eigen::Vector2d(pt.global.x(), pt.global.y()) - posEixo).dot(nPerp);
                secao.terreno.emplace_back(off, pt.cota, "TIN");
            }
        }

        // D. Ordenação
        std::sort(secao.terreno.begin(), secao.terreno.end(), [](const PontoSecao& a, const PontoSecao& b) {
            return a.offset < b.offset;
        });

        // E. Exportação Individual (Nome amigável para Lisp)
        // Ex: Projeto_02_S_00020.txt
        QString nomeSec = dirTrabalho + raizNome + "_S_" + QString("%1").arg((int)pPerfil.estaca, 5, 10, QChar('0')) + ".txt";

        std::vector<std::map<QString, QString>> dadosSecInd;
        for (const auto& pt : secao.terreno) {
            std::map<QString, QString> linha;
            linha["OFF"] = StorageProvider::formatarValor(pt.offset, 12);
            linha["Z"]   = StorageProvider::formatarValor(pt.cota, 12);
            dadosSecInd.push_back(linha);
        }
        std::vector<std::pair<QString, int>> laySecInd = {{"OFF", 12}, {"Z", 12}};
        StorageProvider::exportarFixo(nomeSec, dadosSecInd, laySecInd);
    }

    ui->statusbar->showMessage("Perfil e " + QString::number(m_corredor.vertical.pontos.size()) + " seções geradas.");
//........................................................
/*
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
*/

    ui->statusbar->showMessage("Arquivos gerados em: " + dirTrabalho);
}

Terrapleno::~Terrapleno()
{
    delete ui;
}
