#pragma once
#include "GeometriaBase.h"
#include <map>
#include <list>

struct DadosBrutos
{
    Ponto p1;
    Ponto p2;
    double bulge;
    DadosBrutos(const Ponto& ini = Ponto(), const Ponto& fim = Ponto(), double b = 0.0): p1(ini), p2(fim), bulge(b) {}
};

struct ArestaTIN {
    int iIni, iFim; // Índices no vetor de pontos (mais rápido que IDs de texto)
    ArestaTIN(int p0 = -1, int p1 = -1) : iIni(p0), iFim(p1) {}
};

struct Face {
    int v[3]; // Índices dos 3 pontos que formam o triângulo
    Eigen::Vector3d calcularNormal(const std::vector<Ponto>& pontos) const;
};

struct Superficie {
    QString nome;
    Camada tipo;
    std::vector<Ponto> pontos;
    std::vector<ArestaTIN> arestas;
    std::vector<Face> faces;
    std::vector<int> indicesContorno; // A "costura" que você criou
    std::list<int> iContorno;
    void reconstruirFaces();

    void gerarContornoSequencial();

    double obterZ(double x, double y);
    void processarProjeto(const std::vector<DadosBrutos>& eixo);

    // Função vital para o Perfil: Interpolação Z em uma aresta
    double interpolarZ(int i1, int i2, double x, double y) const;
};
