#pragma onceF
#include <QString>
#include <Eigen/Dense>

//   Camada Ponto, PontoPerfil, PontoSecao, BBox e PontoIntersecaoTIN

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
    Ponto(const QString& id = "", const QString& atr = "", double x = 0.0, double y = 0.0, double z = 0.0)
        : nome(id), atributo(atr), x(x), y(y), z(z)
    {}

    Eigen::Vector3d pos() const     //  construtor simplificado
    {
        return Eigen::Vector3d(x, y, z);
    }
};

struct PontoPerfil
{
    double estaca;
    double cota;
    double distanciaAcumulada;

    PontoPerfil(double s = 0.0, double z = 0.0) : estaca(s), cota(z)
    {}
};

struct PontoSecao
{
    double offset;
    double cota;
    QString codigo;     // "Bordo", "Eixo", "Pé de Talude"

    PontoSecao(double off = 0.0, double z = 0.0, QString cod = ""): offset(off), cota(z), codigo(cod)
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

struct PontoIntersecaoTIN
{
    double estaca;
    double offset;
    double cota;
    Eigen::Vector3d global;

    PontoIntersecaoTIN(double s = 0.0, double off = 0.0, double z = 0.0, const Eigen::Vector3d& g = Eigen::Vector3d::Zero())
        : estaca(s), offset(off), cota(z), global(g)
    {}
};
