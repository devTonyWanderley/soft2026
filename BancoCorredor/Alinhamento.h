#pragma once

#include "Topologia.h"

//  TipoElemento, DadosBrutos, SegmentoHorizontal, EixoHorizontal, PIV, PerfilLongitudinal e GreideProjeto

struct DadosBrutos
{
    Ponto p1;
    Ponto p2;
    double bulge;
    DadosBrutos(const Ponto& ini = Ponto(), const Ponto& fim = Ponto(), double b = 0.0): p1(ini), p2(fim), bulge(b) {}
};

struct SegmentoHorizontal
{
    TipoElemento tipo;
    Ponto pIni;
    Ponto pFim;
    double bulge;
    double raio;
    double delta;
    double comprimento;
    double azimuteEntrada;
    double estacaInicial;
    double estacaFinal;
    SegmentoHorizontal(const Ponto& inicio, const Ponto& fim, double b, double estacaRef)
        : pIni(inicio), pFim(fim), bulge(b), estacaInicial(estacaRef)
    {
        double corda = (pFim.pos2d() - pIni.pos2d()).norm();
        if (std::abs(bulge) < 1e-6)
        {
            tipo = TipoElemento::Reta;
            raio = 0.0;
            delta = 0.0;
            comprimento = corda;
        }
        else
        {
            tipo = TipoElemento::Curva;
            delta = 4.0 * std::atan(std::abs(bulge));
            raio = (corda / (2.0 * std::sin(delta / 2.0)));
            comprimento = raio * delta;
        }
        estacaFinal = estacaInicial + comprimento;
    }

    bool eCurva() const
    {
        return std::abs(bulge) > 1e-6;
    }

    Eigen::Vector2d calcularCentro() const;
    BBox calcularBBox() const;
    std::vector<PontoIntersecaoTIN> interceptarArco(const Ponto& a1, const Ponto& a2) const;
    std::vector<PontoIntersecaoTIN> interceptarReta(const Ponto& a1, const Ponto& a2) const;
    Eigen::Vector2d calcularPerpendicularLocal(double s) const;
    Eigen::Vector2d getXYNaEstaca(double s) const;
    static std::vector<PontoIntersecaoTIN> interceptarRetaManual(const Ponto& p1, const Ponto& p2, const Ponto& a1, const Ponto& a2);
};

class EixoHorizontal
{
public:
    std::vector<SegmentoHorizontal> trechos;
    double estacaPartida;

    EixoHorizontal() : estacaPartida(0.0) {}
    EixoHorizontal(const std::vector<DadosBrutos>& entrada, double estacaZero = 0.0): estacaPartida(estacaZero)
    {
        double estacaAtual = estacaPartida;
        for (const auto& d : entrada)
        {
            trechos.emplace_back(d.p1, d.p2, d.bulge, estacaAtual);
            estacaAtual = trechos.back().estacaFinal;
        }
    }
    Eigen::Vector2d getPerpendicularNaEstaca(double s) const;
    Eigen::Vector2d getXYNaEstaca(double s) const;
};

struct PIV
{
    double estaca;
    double cota;
    double k;
    double comprimentoCurva;
};

struct GreideProjeto : public PerfilLongitudinal
{
    std::vector<PIV> vertices;
    double cotaNaEstaca(double s) const;
};
