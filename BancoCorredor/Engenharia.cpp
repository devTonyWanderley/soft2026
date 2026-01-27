#include "Engenharia.h"
#include "StorageProvider.h"
#include <algorithm>
#include <cmath>
#include <map>
#include <set>

#include <QDebug>

void Corredor::gerarPerfilEConsolidar(const Superficie& terreno) {
    this->vertical.pontos.clear();
    this->vertical.pontosProjeto.clear();

    std::vector<PontoPerfil> lista;

    // 1. ACIDENTES (A real variação do terreno)
    for (const auto& seg : horizontal.trechos) {
        for (const auto& ar : terreno.arestas) {
            auto inters = SegmentoHorizontal::interceptarRetaManual(
                seg.pIni, seg.pFim, terreno.pontos[ar.iIni], terreno.pontos[ar.iFim]);
            for (const auto& pt : inters) {
                // Aqui o Z é REAL, veio da aresta
                lista.emplace_back(pt.estaca, pt.cota, "");
            }
        }
    }

    // 2. PROJETO (Estacas redondas e Notáveis)
    for (const auto& seg : horizontal.trechos) {
        lista.emplace_back(seg.estacaInicial, 0.0, "PC");
        lista.emplace_back(seg.estacaFinal,   0.0, "PT");

        double passo = (seg.tipo == TipoElemento::Reta) ? 10.0 : 5.0;
        double s = std::ceil(seg.estacaInicial / passo) * passo;
        while (s < (seg.estacaFinal - 0.001)) {
            if ((s - seg.estacaInicial) > 1.0) lista.emplace_back(s, 0.0, "INT");
            s += passo;
        }
    }

    // 3. ORDENAÇÃO
    std::sort(lista.begin(), lista.end(), [](const PontoPerfil& a, const PontoPerfil& b) {
        return a.estaca < b.estaca;
    });

    // 4. LIMPEZA E PREENCHIMENTO (A "Zebra" do Z constante mora aqui)
    for (size_t i = 0; i < lista.size(); ++i) {
        // Se for ponto de projeto (Z=0), ele "rouba" a cota do acidente mais próximo
        if (lista[i].cota < 0.001) {
            // Busca o vizinho mais próximo que tenha cota (frente ou trás)
            lista[i].cota = buscarCotaVizinha(lista, i);
        }

        // Evita duplicatas coladas (< 1cm)
        if (vertical.pontos.empty() || (lista[i].estaca - vertical.pontos.back().estaca) > 0.01) {
            vertical.pontos.push_back(lista[i]);
            if (!lista[i].tipo.isEmpty()) vertical.pontosProjeto.push_back(lista[i]);
        } else if (!lista[i].tipo.isEmpty()) {
            // Se coincidir, garante que o TIPO prevaleça
            vertical.pontos.back().tipo = lista[i].tipo;
        }
    }
}

double buscarCotaVizinha(const std::vector<PontoPerfil>& lista, size_t idx) {
    // Tenta achar cota nos vizinhos imediatos para evitar a função cotaNaEstaca quebrada
    if (idx > 0 && lista[idx-1].cota > 0.1) return lista[idx-1].cota;
    if (idx < lista.size()-1 && lista[idx+1].cota > 0.1) return lista[idx+1].cota;
    return 500.0; // Fallback se nada for achado
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
