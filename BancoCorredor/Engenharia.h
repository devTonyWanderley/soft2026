#pragma once

#include "Alinhamento.h"
#include <set>

//  SecaoTransversal e Corredor

struct SecaoTransversal
{
    double estaca;
    Ponto centroEixo;
    double azimute;
    double limiteEsq;
    double limiteDir;
    std::vector<PontoSecao> terreno;
    std::vector<PontoSecao> projeto;
    double areaCorte;
    double areaAterro;

    SecaoTransversal(double s = 0.0): estaca(s), limiteEsq(0.0), limiteDir(0.0), areaCorte(0.0), areaAterro(0.0)
    {}
    Eigen::Vector2d vetorDiretor() const
    {
        double rad = (90.0 - azimute) * (M_PI / 180.0);
        return Eigen::Vector2d(cos(rad), sin(rad));
    }
};

class Corredor
{
public:
    Corredor() = default;
    EixoHorizontal horizontal;
    PerfilLongitudinal vertical;
    std::vector<SecaoTransversal> secoes;
    void gerarPerfilLongitudinal(const Superficie& terreno);
    void consolidarEstaqueamentoLongitudinal();
    void exportarDados(const QString& caminho, Camada tipo);
    void processar(const Superficie& terreno);
    double calcularVolumeTotal();
    void gerarAmostragemTIN(const Superficie& terreno, double larguraBusca);
};
