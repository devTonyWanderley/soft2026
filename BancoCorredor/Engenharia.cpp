#include "Engenharia.h"
#include "StorageProvider.h"
#include <algorithm>
#include <cmath>
#include <map>
#include <set>

#include <QDebug>

// 1. GERAÇÃO DO PERFIL E CONSOLIDAÇÃO DAS ESTACAS
void Corredor::gerarPerfilEConsolidar(const Superficie& terreno) {
    //  --RASTREIO--    .................................................
    qDebug() << "--- DEBUG INÍCIO PERFIL ---";
    qDebug() << "Arestas na Malha:" << terreno.arestas.size();
    if(!terreno.pontos.empty())
        qDebug() << "Exemplo Ponto 0 Z:" << terreno.pontos[0].z;
    //  .................................................................
    this->vertical.pontos.clear();
    this->vertical.pontosProjeto.clear();

    // Mapa para garantir que cada estaca tenha sua cota e tipo
    // double (estaca) -> pair<cota, tipo>
    std::map<double, std::pair<double, QString>> perfilCompleto;

    // A. Estacas de Projeto (PC, PT, INT)
    for (const auto& seg : horizontal.trechos) {
        double p1 = seg.estacaInicial;
        double p2 = seg.estacaFinal;

        perfilCompleto[p1] = {0.0, "PC"}; // Cota será preenchida depois
        perfilCompleto[p2] = {0.0, "PT"};

        double passo = (seg.tipo == TipoElemento::Reta) ? 10.0 : 5.0;
        double s = std::ceil(p1 / passo) * passo;
        if (std::abs(s - p1) < 0.001) s += passo;

        while (s < (p2 - 0.001)) {
            if ((s - p1) > 2.5 && (p2 - s) > 2.5) {
                perfilCompleto[s] = {0.0, "INT"};
            }
            s += passo;
        }
    }
    //  --RASTREIO--    .................................................
    // Rastreio da Interseção
    int encontrados = 0;
    //  .................................................................

    // B. Acidentes Geográficos (Interseção com Arestas)
    for (const auto& seg : horizontal.trechos) {
        for (const auto& ar : terreno.arestas) {
            auto inters = SegmentoHorizontal::interceptarRetaManual(
                seg.pIni, seg.pFim, terreno.pontos[ar.iIni], terreno.pontos[ar.iFim]);

            for (const auto& pt : inters) {
                //  --RASTREIO--    .................................................
                encontrados++;
                // Log do primeiro acidente para validar valores
                if(encontrados <= 5) {
                    qDebug() << "Acidente" << encontrados << "-> Est:" << pt.estaca << "Z:" << pt.cota;
                }
                // GARANTIA: Inserção direta no mapa
                perfilCompleto[pt.estaca] = std::make_pair(pt.cota, QString(""));
                //  .................................................................
                // Se a estaca já existe (é de projeto), mantemos o tipo,
                // senão entra com tipo vazio ""
                if (perfilCompleto.find(pt.estaca) == perfilCompleto.end()) {
                    perfilCompleto[pt.estaca] = {pt.cota, ""};
                } else {
                    perfilCompleto[pt.estaca].first = pt.cota;
                }
            }
            //  --RASTREIO--    .................................................
            qDebug() << "Total de Acidentes mapeados:" << encontrados;
            //  .................................................................
        }
    }

    // C. Povoar os vetores finais e Corrigir Cotas Z
    for (auto& [est, dados] : perfilCompleto) {
        // Se a cota ainda estiver zerada (ponto de projeto), interpolamos na malha
        if (dados.first == 0.0) {
            dados.first = vertical.cotaNaEstaca(est);
        }

        PontoPerfil p(est, dados.first, dados.second);
        vertical.pontos.push_back(p);

        if (!dados.second.isEmpty()) {
            vertical.pontosProjeto.push_back(p);
        }
    }
}

// 2. GERAÇÃO E SALVAMENTO DE SEÇÕES INDIVIDUAIS
void Corredor::gerarArquivosSecoes(const Superficie& terreno, const QString& dirBase, const QString& raiz) {
    for (const auto& pProj : vertical.pontosProjeto) {
        SecaoTransversal secao(pProj.estaca);
        Eigen::Vector2d pos = horizontal.getXYNaEstaca(pProj.estaca);
        Eigen::Vector2d nPerp = horizontal.getPerpendicularNaEstaca(pProj.estaca);

        // Limites pelo Contorno (Raycasting nos 101 pontos)
        double dEsq = StorageProvider::calcularDistanciaAoContorno(pos, -nPerp, terreno);
        double dDir = StorageProvider::calcularDistanciaAoContorno(pos, nPerp, terreno);

        Ponto pEsq("", "", pos.x() - nPerp.x()*dEsq, pos.y() - nPerp.y()*dEsq);
        Ponto pDir("", "", pos.x() + nPerp.x()*dDir, pos.y() + nPerp.y()*dDir);

        for (const auto& ar : terreno.arestas) {
            auto inters = SegmentoHorizontal::interceptarRetaManual(pEsq, pDir, terreno.pontos[ar.iIni], terreno.pontos[ar.iFim]);
            for (const auto& pt : inters) {
                double off = (Eigen::Vector2d(pt.global.x(), pt.global.y()) - pos).dot(nPerp);
                secao.terreno.emplace_back(off, pt.cota, "TIN");
            }
        }

        std::sort(secao.terreno.begin(), secao.terreno.end(), [](const PontoSecao& a, const PontoSecao& b) {
            return a.offset < b.offset;
        });

        // Exportação via StorageProvider
        QString path = dirBase + raiz + "_S_" + QString("%1").arg((int)pProj.estaca, 5, 10, QChar('0')) + ".txt";
        StorageProvider::exportarSecaoIndividual(path, pProj.estaca, secao.terreno);
    }
}

void Corredor::exportarPerfilIdentificado(const QString& caminho) {
    std::vector<std::map<QString, QString>> dados;

    for (const auto& p : vertical.pontos) {
        std::map<QString, QString> linha;
        linha["EST"]  = StorageProvider::formatarValor(p.estaca, 12);
        linha["T_Z"]  = StorageProvider::formatarTexto("COTA_Z", 16);
        linha["Z"]    = StorageProvider::formatarValor(p.cota, 12);
        linha["T_ID"] = StorageProvider::formatarTexto("TIPO", 16);
        linha["ID"]   = StorageProvider::formatarTexto(p.tipo, 12);
        dados.push_back(linha);
    }

    // Layout padrão: 12 | 16 | 12 | 16 | 12
    std::vector<std::pair<QString, int>> lay = {
        {"EST", 12}, {"T_Z", 16}, {"Z", 12}, {"T_ID", 16}, {"ID", 12}
    };

    StorageProvider::exportarFixo(caminho, dados, lay);
}
