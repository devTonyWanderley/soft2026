#pragma once
#include "Alinhamento.h"
#include "Topologia.h"

struct SecaoTransversal {
    double estaca;
    std::vector<PontoSecao> terreno;
    std::vector<PontoSecao> projeto;

    double areaCorte = 0.0;
    double areaAterro = 0.0;

    SecaoTransversal(double s = 0.0) : estaca(s) {}
};

class Corredor {
public:
    Corredor() = default;

    EixoHorizontal horizontal;
    PerfilLongitudinal vertical;
    std::vector<SecaoTransversal> secoes;

    // 1. Processamento Longitudinal (Gera o Perfil Fiel + Estacas de Projeto)
    void gerarPerfilEConsolidar(const Superficie& terreno);

    // 2. Processamento Transversal (Gera e Salva os arquivos Individuais)
    void gerarArquivosSecoes(const Superficie& terreno, const QString& dirBase, const QString& raiz);

    // 3. Exportação do Perfil (O arquivo que o Lisp vai ler para desenhar o "pente")
    void exportarPerfilIdentificado(const QString& caminho);

    // Futuro: Métodos de Volume entrarão aqui
};
