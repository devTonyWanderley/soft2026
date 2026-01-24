#ifndef REVTIPOS_H
#define REVTIPOS_H

#include <QString>
#include <Eigen/Dense>
#include <map>

enum class TipoElemento
{
    Reta,
    Curva
};

enum class Camada
{
    Primitiva,
    Projeto,
    Subleito,
    Corte,
    Aterro,
    Desmonte
};

struct Ponto
{
    QString nome;
    QString atributo;
    double x;
    double y;
    double z;

    // Construtor otimizado
    Ponto(const QString& id = "", const QString& atr = "", double x = 0.0, double y = 0.0, double z = 0.0)
        : nome(id), atributo(atr), x(x), y(y), z(z)
    {}

    // Dica extra: Adicione um construtor de cópia simplificado para Eigen
    Eigen::Vector3d pos() const
    {
        return Eigen::Vector3d(x, y, z);
    }
};

struct PontoIntersecaoTIN
{
    double estaca;           // Posição longitudinal no eixo
    double offset;           // Distância lateral (0.0 no perfil, != 0.0 na seção)
    double cota;             // Z interpolado na TIN
    Eigen::Vector3d global;  // X, Y, Z absolutos (essencial para conferência)

    // Construtor auxiliar para facilitar o uso no código
    PontoIntersecaoTIN(double s = 0.0, double off = 0.0, double z = 0.0, const Eigen::Vector3d& g = Eigen::Vector3d::Zero())
        : estaca(s), offset(off), cota(z), global(g)
    {}
};

struct BBox {
    double minX, maxX, minY, maxY;

    BBox() : minX(1e15), maxX(-1e15), minY(1e15), maxY(-1e15) {}

    void atualizar(double x, double y)
    {
        if (x < minX) minX = x;
        if (x > maxX) maxX = x;

        if (y < minY) minY = y;
        if (y > maxY) maxY = y;
    }

    bool intercepta(const BBox& outra) const
    {
        return !(outra.minX > maxX || outra.maxX < minX || outra.minY > maxY || outra.maxY < minY);
    }
};

// Representa uma linha bruta vinda do arquivo (ex: uma linha do PDW)
// que será convertida em um segmento geométrico real.
struct DadosBrutos
{
    Ponto p1;
    Ponto p2;
    double bulge;

    DadosBrutos(const Ponto& ini = Ponto(), const Ponto& fim = Ponto(), double b = 0.0)
        : p1(ini), p2(fim), bulge(b) {}
};

struct SegmentoHorizontal
{
    TipoElemento tipo;

    // Dados Geométricos (Vindos do Lisp/DXF)
    Ponto pIni;      // X, Y inicial (PC ou PP)
    Ponto pFim;      // X, Y final (PT ou PF)
    double bulge;    // Código 42 (0 para retas)

    // Dados Calculados (Otimização para Estaqueamento)
    double raio;
    double delta;    // Ângulo central
    double comprimento; // Desenvolvimento (L)
    double azimuteEntrada;

    double estacaInicial;
    double estacaFinal;
    SegmentoHorizontal(const Ponto& inicio, const Ponto& fim, double b, double estacaRef)
        : pIni(inicio), pFim(fim), bulge(b), estacaInicial(estacaRef)
    {
        // 1. Distância da Corda usando Eigen
        double corda = (pFim.pos() - pIni.pos()).head<2>().norm();

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
            // 2. Fórmulas de Bulge para Curva
            delta = 4.0 * std::atan(std::abs(bulge));
            raio = (corda / (2.0 * std::sin(delta / 2.0)));
            comprimento = raio * delta;
        }
        estacaFinal = estacaInicial + comprimento;
    }

    // Função de verificação rápida
    bool eCurva() const
    {
        return std::abs(bulge) > 1e-6;
    }

    Eigen::Vector2d calcularCentro() const;

    BBox calcularBBox() const;

    std::vector<PontoIntersecaoTIN> interceptarArco(const Ponto& a1, const Ponto& a2) const;
    std::vector<PontoIntersecaoTIN> interceptarReta(const Ponto& a1, const Ponto& a2) const;
};

struct ColunaExport
{
    QString titulo;
    int largura;
    bool isNumero;
};

class EixoHorizontal
{
public:
    // ADICIONE ESTE: Construtor padrão (vazio)
    EixoHorizontal() : estacaPartida(0.0) {}
    std::vector<SegmentoHorizontal> trechos;
    double estacaPartida;

    // Construtor que recebe os dados brutos e "monta" o quebra-cabeça
    EixoHorizontal(const std::vector<DadosBrutos>& entrada, double estacaZero = 0.0): estacaPartida(estacaZero)
    {
        double estacaAtual = estacaPartida;
        for (const auto& d : entrada)
        {
            // Cria o segmento e ele já se auto-calcula
            trechos.emplace_back(d.p1, d.p2, d.bulge, estacaAtual);
            // Atualiza a estaca para o próximo segmento
            estacaAtual = trechos.back().estacaFinal;
        }
    }
};

struct ArestaTIN
{
    int iIni;  // Índice do ponto inicial no vetor global de pontos
    int iFim;  // Índice do ponto final no vetor global de pontos

    // Para algoritmos de TIN (como Delaunay),
    // costuma-se guardar as faces vizinhas
    int faceEsquerda = -1;
    int faceDireita  = -1;

    ArestaTIN(int p0 = -1, int p1 = -1) : iIni(p0), iFim(p1)
    {}

    // No Qt, para garantir que não criemos arestas duplicadas (A->B e B->A)
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
    int v[3]; // Índices dos 3 pontos no vetor global std::vector<Ponto>

    // Dados calculados (Cachê para performance)
    double declividade; // Inclinação máxima da face (%)
    double areaHorizontal;

    // Construtor usando a lista de inicialização
    Face(int a = -1, int b = -1, int c = -1)
    {
        v[0] = a;
        v[1] = b;
        v[2] = c;
    }

    // O "Pulo do Gato": Vetor Normal usando Eigen
    // Essencial para saber para onde a água corre e para volumes
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
    QString nome;         // Nome amigável para a interface Qt (ex: "Terreno Natural")
    Camada tipo;          // Identificador lógico para cálculos de volume

    std::vector<Ponto> pontos; // Todos os vértices desta superfície
    std::vector<ArestaTIN> arestas;
    std::vector<Face> faces;   // A malha TIN que conecta os pontos

    // Metadados calculados uma única vez
    double areaTotal;
    double volumeAcumulado;

    Superficie(QString n = "", Camada t = Camada::Primitiva): nome(n), tipo(t), areaTotal(0.0), volumeAcumulado(0.0)
    {}
};

struct PontoPerfil
{
    double estaca;
    double cota;
    double distanciaAcumulada; // Útil para cálculos de rampa real

    PontoPerfil(double s = 0.0, double z = 0.0) : estaca(s), cota(z)
    {}
};

struct PerfilLongitudinal
{
    Camada tipo;                // Identifica se é Terreno, Greide de Projeto, etc.
    std::vector<PontoPerfil> pontos;

    // Metadados para o gráfico (Data Visualization no Qt)
    double cotaMinima;
    double cotaMaxima;
    double extensaoTotal;

    // Função Crítica: Retorna a cota exata em uma estaca qualquer por interpolação linear
    double cotaNaEstaca(double s) const;
};

struct PIV
{
    double estaca;
    double cota;
    double k;         // Parâmetro de curvatura vertical (Kv)
    double comprimentoCurva; // L
};

struct GreideProjeto : public PerfilLongitudinal
{
    std::vector<PIV> vertices;

    // Sobrescreve a interpolação para usar as fórmulas de parábola nas curvas
    double cotaNaEstaca(double s) const;
};

struct PontoSecao
{
    double offset;      // Negativo = Esquerda, Positivo = Direita
    double cota;        // Altitude absoluta (m)
    QString codigo;     // "Bordo", "Eixo", "Pé de Talude"

    PontoSecao(double off = 0.0, double z = 0.0, QString cod = ""): offset(off), cota(z), codigo(cod)
    {}
};

struct SecaoTransversal
{
    double estaca;
    Ponto centroEixo;           // Coordenada global (X,Y,Z) vinda do Traçado Horizontal/Vertical
    double azimute;             // Direção normal para projeção dos offsets

    std::vector<PontoSecao> terreno; // Pontos extraídos da TIN
    std::vector<PontoSecao> projeto; // Pontos do gabarito (Pista, Taludes)

    // Resultados de cálculo para a DMT
    double areaCorte;
    double areaAterro;

    SecaoTransversal(double s = 0.0) : estaca(s), areaCorte(0.0), areaAterro(0.0)
    {}
};

class ExportEngine
{
public:
    // Rotina genérica para exportar qualquer lista de dados em formato fixo
    static bool salvarFixo(const QString& caminho,
                           const std::vector<std::map<QString, QString>>& linhas,
                           const std::vector<ColunaExport>& layout);

    // Utilitários de formatação (o "coração" da parametrização)
    static QString formatarValor(double val, int largura);
    static QString formatarTexto(QString txt, int largura);
};

class Corredor
{
public:
    // Força o compilador a gerar o construtor padrão
    Corredor() = default;
    EixoHorizontal horizontal;
    PerfilLongitudinal vertical;
    std::vector<SecaoTransversal> secoes;
    void gerarPerfilLongitudinal(const Superficie& terreno);
    void consolidarEstaqueamentoLongitudinal();
    void exportarDados(const QString& caminho, Camada tipo);

    // Métodos que você construirá em casa:
    void processar(const Superficie& terreno);
    double calcularVolumeTotal(); // Integração das áreas das seções
};


#endif // REVTIPOS_H
