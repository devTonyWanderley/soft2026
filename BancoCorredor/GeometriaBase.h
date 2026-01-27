#pragma once
#include <QString>
#include <Eigen/Dense>
#include <vector>

// 1. Identificadores de Projeto
enum class Camada { Primitiva, Projeto, Subleito, Corte, Aterro };
enum class TipoElemento { Reta, Curva };

// 2. O Ponto Geográfico (Base para TIN e Eixo)
struct Ponto {
    QString nome;
    QString atributo;
    double x, y, z;

    Ponto(const QString& n = "", const QString& a = "", double x = 0.0, double y = 0.0, double z = 0.0)
        : nome(n), atributo(a), x(x), y(y), z(z) {}

    Eigen::Vector2d pos2d() const { return Eigen::Vector2d(x, y); }
    Eigen::Vector3d pos3d() const { return Eigen::Vector3d(x, y, z); }
};

// 3. Estruturas para Perfil e Seções (Com identificação de tipo para o Lisp)
struct PontoPerfil {
    double estaca;
    double cota;
    QString tipo; // "PC", "PT", "INT" ou "" (para acidentes geográficos)

    PontoPerfil(double s = 0.0, double z = 0.0, QString t = "")
        : estaca(s), cota(z), tipo(t) {}
};

struct PontoSecao {
    double offset;
    double cota;
    QString codigo;

    PontoSecao(double off = 0.0, double z = 0.0, QString cod = "")
        : offset(off), cota(z), codigo(cod) {}
};

// 4. Utilitário de Caixa Envolvente (Para otimizar buscas na malha)
struct BBox {
    double minX, maxX, minY, maxY;
    BBox() : minX(1e15), maxX(-1e15), minY(1e15), maxY(-1e15) {}
    void atualizar(double x, double y) {
        if (x < minX) minX = x; if (x > maxX) maxX = x;
        if (y < minY) minY = y; if (y > maxY) maxY = y;
    }

    // ADD THIS FUNCTION:
    bool intercepta(const BBox& outra) const {
        return !(outra.minX > maxX ||
                 outra.maxX < minX ||
                 outra.minY > maxY ||
                 outra.maxY < minY);
    }
};

// No GeometriaBase.h
struct PontoIntersecaoTIN {
    double estaca;
    double offset;
    double cota;
    Eigen::Vector3d global;

    PontoIntersecaoTIN() : estaca(0), offset(0), cota(0), global(0,0,0) {}
};

// No GeometriaBase.h
struct PerfilLongitudinal {
    std::vector<PontoPerfil> pontos;        // Acidentes + Projeto (Para a Polilinha)
    std::vector<PontoPerfil> pontosProjeto; // APENAS PC, PT e INT (Para Seções e Chamadas)

    // Função que busca a cota Z interpolando entre os pontos do terreno
    double cotaNaEstaca(double s) const {
        if (pontos.empty()) return 0.0;
        // Busca simples (como estamos com calma, depois otimizamos com binary search)
        for (size_t i = 0; i < pontos.size() - 1; ++i) {
            if (s >= pontos[i].estaca && s <= pontos[i+1].estaca) {
                double dist = pontos[i+1].estaca - pontos[i].estaca;
                if (dist < 1e-6) return pontos[i].cota;
                double t = (s - pontos[i].estaca) / dist;
                return pontos[i].cota + t * (pontos[i+1].cota - pontos[i].cota);
            }
        }
        return pontos.back().cota;
    }
};
