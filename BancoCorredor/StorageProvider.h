#pragma once
#include "Alinhamento.h"
#include <QString>
#include <vector>
#include <map>

// Centraliza as configurações de Layout (Largura Fixa, Escala, etc)
struct FileLayout
{
    bool larguraFixa = true;
    double escala = 1.0;
    std::map<QString, std::pair<int, int>> colunas;
};

class StorageProvider
{
public:
    // Métodos estáticos para leitura (Input)
    static std::vector<Ponto> lerPontos(const QString& path, const FileLayout& layout);
    static std::vector<DadosBrutos> lerEixo(const QString& path, const FileLayout& layout);
    static void lerArestas(const QString& path, Superficie& superf);

    // ADICIONE ESTA LINHA:
    static bool exportarSecaoIndividual(const QString& path, double estaca, const std::vector<PontoSecao>& pontos);

    static QString formatarValor(double val, int largura);
    static QString formatarTexto(QString txt, int largura);

    // Métodos para escrita (Output) - Parametrizado como discutimos
    static bool exportarFixo(const QString& path,
                             const std::vector<std::map<QString, QString>>& dados,
                             const std::vector<std::pair<QString, int>>& layout);
    static double calcularDistanciaAoContorno(const Eigen::Vector2d& origem,
                                              const Eigen::Vector2d& direcao,
                                              const Superficie& terreno);
};
