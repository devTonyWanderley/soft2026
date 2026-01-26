//  SecaoTransversal e Corredor

#include "Engenharia.h"
#include <map>
#include "StorageProvider.h"

void Corredor::gerarPerfilLongitudinal(const Superficie& terreno)
{
    this->vertical.pontos.clear();
    std::vector<PontoIntersecaoTIN> todasIntersecoes;
    for (const auto& seg : horizontal.trechos)
    {
        BBox boxSeg = seg.calcularBBox();
        for (const auto& aresta : terreno.arestas)
        {
            const Ponto& a1 = terreno.pontos[aresta.iIni];
            const Ponto& a2 = terreno.pontos[aresta.iFim];
            BBox boxAresta;
            boxAresta.atualizar(a1.x, a1.y);
            boxAresta.atualizar(a2.x, a2.y);
            if (!boxSeg.intercepta(boxAresta)) continue;
            std::vector<PontoIntersecaoTIN> pts;
            if (seg.tipo == TipoElemento::Reta)
            {
                pts = seg.interceptarReta(a1, a2);
            }
            else
            {
                pts = seg.interceptarArco(a1, a2);
            }
            todasIntersecoes.insert(todasIntersecoes.end(), pts.begin(), pts.end());
        }
    }
    std::sort(todasIntersecoes.begin(), todasIntersecoes.end(),[](const PontoIntersecaoTIN& a, const PontoIntersecaoTIN& b)
              {
                  return a.estaca < b.estaca;
              });
    for (const auto& it : todasIntersecoes)
    {
        this->vertical.pontos.emplace_back(it.estaca, it.cota);
    }
}

void Corredor::consolidarEstaqueamentoLongitudinal()
{
    std::set<double> estacasMestre;
    for (const auto& p : vertical.pontos)
    {
        estacasMestre.insert(p.estaca);
    }
    for (const auto& seg : horizontal.trechos)
    {
        estacasMestre.insert(seg.estacaInicial);
        estacasMestre.insert(seg.estacaFinal);
        double passo = (seg.tipo == TipoElemento::Reta) ? 10.0 : 5.0;
        double s = std::ceil(seg.estacaInicial / passo) * passo;
        while (s < seg.estacaFinal)
        {
            estacasMestre.insert(s);
            s += passo;
        }
    }
    std::vector<PontoPerfil> perfilFinal;
    for (double s : estacasMestre)
    {
        double z = vertical.cotaNaEstaca(s);
        perfilFinal.emplace_back(s, z);
    }
    vertical.pontos = std::move(perfilFinal);
}

void Corredor::exportarDados(const QString& caminho, Camada tipo)
{
    std::vector<std::map<QString, QString>> linhas;
    for (const auto& p : vertical.pontos)
    {
        std::map<QString, QString> linha;
        linha["NOME"] = StorageProvider::formatarTexto("ESTACA", 16);
        linha["VALOR"] = StorageProvider::formatarValor(p.estaca, 12);
        linha["NOME2"] = StorageProvider::formatarTexto("COTA_Z", 16);
        linha["VALOR2"] = StorageProvider::formatarValor(p.cota, 12);
        linhas.push_back(linha);
    }
    /*
    // Define o layout usando o par {Nome da Chave, Largura da Coluna}
    std::vector<std::pair<QString, int>> layout = {
        {"NOME", 16},
        {"VALOR", 12},
        {"NOME2", 16},
        {"VALOR2", 12}
    };

    StorageProvider::exportarFixo(caminho, linhas, layout);

    */
    //std::vector<ColunaExport> layout = {{"NOME", 16, false}, {"VALOR", 12, true}, {"NOME2", 16, false}, {"VALOR2", 12, true}};

    std::vector<std::pair<QString, int>> layout = {
        {"NOME", 16},
        {"VALOR", 12},
        {"NOME2", 16},
        {"VALOR2", 12}
    };
    StorageProvider::exportarFixo(caminho, linhas, layout);
}

void Corredor::gerarAmostragemTIN(const Superficie& terreno, double larguraBusca)
{
    this->secoes.clear();
    for (const auto& pPerfil : vertical.pontos)
    {
        SecaoTransversal secao(pPerfil.estaca);
        Eigen::Vector2d posEixo = horizontal.getXYNaEstaca(pPerfil.estaca);
        Eigen::Vector2d nPerp = horizontal.getPerpendicularNaEstaca(pPerfil.estaca);
        Ponto pEsq("", "", posEixo.x() - nPerp.x() * larguraBusca, posEixo.y() - nPerp.y() * larguraBusca);
        Ponto pDir("", "", posEixo.x() + nPerp.x() * larguraBusca, posEixo.y() + nPerp.y() * larguraBusca);
        secao.centroEixo = Ponto("", "", posEixo.x(), posEixo.y(), pPerfil.cota);
        for (const auto& aresta : terreno.arestas)
        {
            auto inters = SegmentoHorizontal::interceptarRetaManual(pEsq, pDir, terreno.pontos[aresta.iIni], terreno.pontos[aresta.iFim]);
            for (const auto& pt : inters)
            {
                Eigen::Vector2d vInt(pt.global.x(), pt.global.y());
                double offset = (vInt - posEixo).dot(nPerp);
                secao.terreno.emplace_back(offset, pt.cota, "TIN");
            }
        }
        std::sort(secao.terreno.begin(), secao.terreno.end(), [](const PontoSecao& a, const PontoSecao& b){return a.offset < b.offset;});
        this->secoes.push_back(secao);
    }
}
