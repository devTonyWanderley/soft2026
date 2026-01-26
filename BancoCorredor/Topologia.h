#pragma once

#include "GeometriaBase.h"

//  ArestaTIN, Face, Superficie

struct ArestaTIN
{
    int iIni;
    int iFim;
    int faceEsquerda = -1;
    int faceDireita  = -1;

    ArestaTIN(int p0 = -1, int p1 = -1) : iIni(p0), iFim(p1)
    {}
    bool operator<(const ArestaTIN& outra) const
    {
        int minA = std::min(iIni, iFim);
        int maxA = std::max(iIni, iFim);
        int minB = std::min(outra.iIni, outra.iFim);
        int maxB = std::max(outra.iIni, outra.iFim);
        return std::tie(minA, maxA) < std::tie(minB, maxB);
    }
};

struct Face
{
    int v[3];
    double declividade;
    double areaHorizontal;

    Face(int a = -1, int b = -1, int c = -1)
    {
        v[0] = a;
        v[1] = b;
        v[2] = c;
    }
    Eigen::Vector3d calcularNormal(const std::vector<Ponto>& pontos) const
    {
        Eigen::Vector3d a = pontos[v[0]].pos();
        Eigen::Vector3d b = pontos[v[1]].pos();
        Eigen::Vector3d c = pontos[v[2]].pos();
        return (b - a).cross(c - a).normalized();
    }
};

struct Superficie
{
    QString nome;
    Camada tipo;
    std::vector<Ponto> pontos;
    std::vector<ArestaTIN> arestas;
    std::vector<Face> faces;
    std::vector<int> indicesContorno;
    double areaTotal;
    double volumeAcumulado;
    Superficie(QString n = "", Camada t = Camada::Primitiva): nome(n), tipo(t), areaTotal(0.0), volumeAcumulado(0.0)
    {}
    void gerarContornoSequencial();
};
