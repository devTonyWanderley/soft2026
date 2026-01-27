#include "Alinhamento.h"
//  TipoElemento, DadosBrutos, SegmentoHorizontal, EixoHorizontal, PIV, PerfilLongitudinal e GreideProjeto

Eigen::Vector2d SegmentoHorizontal::calcularCentro() const
{
    Eigen::Vector2d p1(pIni.x, pIni.y);
    Eigen::Vector2d p2(pFim.x, pFim.y);
    Eigen::Vector2d vCorda = p2 - p1;
    double dCorda = vCorda.norm();
    Eigen::Vector2d pMedio = p1 + vCorda * 0.5;
    double bAbs = std::abs(bulge);
    if (bAbs < 1e-8) return pMedio;
    double f = (dCorda / 2.0) * bAbs;
    double h = ( (dCorda * dCorda) / (8.0 * f) ) - (f / 2.0);
    Eigen::Vector2d vPerp(-vCorda.y() / dCorda, vCorda.x() / dCorda);
    if (bulge < 0) vPerp = -vPerp;
    return pMedio + vPerp * h;
}

BBox SegmentoHorizontal::calcularBBox() const
{
    BBox box;
    box.atualizar(pIni.x, pIni.y);
    box.atualizar(pFim.x, pFim.y);
    if (tipo == TipoElemento::Curva)
    {
        Eigen::Vector2d centro = calcularCentro();
        double r = raio;
        double angIni = std::atan2(pIni.y - centro.y(), pIni.x - centro.x());
        double angFim = std::atan2(pFim.y - centro.y(), pFim.x - centro.x());
        if (bulge < 0 && angFim > angIni) angIni += 2.0 * M_PI;
        if (bulge > 0 && angIni > angFim) angFim += 2.0 * M_PI;
        double aMin = std::min(angIni, angFim);
        double aMax = std::max(angIni, angFim);
        for (double q : {0.0, M_PI/2.0, M_PI, 3.0*M_PI/2.0, 2.0*M_PI})
        {
            if (q >= aMin && q <= aMax)
            {
                box.atualizar(centro.x() + r * cos(q), centro.y() + r * sin(q));
            }
        }
    }
    return box;
}

std::vector<PontoIntersecaoTIN> SegmentoHorizontal::interceptarArco(const Ponto& a1, const Ponto& a2) const
{
    std::vector<PontoIntersecaoTIN> resultados;
    Eigen::Vector2d C = calcularCentro();
    Eigen::Vector2d A(a1.x, a1.y);
    Eigen::Vector2d B(a2.x, a2.y);
    Eigen::Vector2d V = B - A;
    Eigen::Vector2d D = A - C;
    double a = V.dot(V);
    double b = 2.0 * V.dot(D);
    double c = D.dot(D) - (raio * raio);
    double delta = b * b - 4.0 * a * c;
    if (delta < 0) return resultados;
    double t_valores[2] = { (-b + sqrt(delta)) / (2.0 * a), (-b - sqrt(delta)) / (2.0 * a) };
    for (double t : t_valores)
    {
        if (t >= 0.0 && t <= 1.0)
        {
            Eigen::Vector2d pInt = A + t * V;
            double angInt = atan2(pInt.y() - C.y(), pInt.x() - C.x());
            double angIni = atan2(pIni.y - C.y(), pIni.x - C.x());
            double angFim = atan2(pFim.y - C.y(), pFim.x - C.x());
            double aInt = angInt;
            if (bulge > 0)
            {
                if (angFim < angIni) angFim += 2.0 * M_PI;
                if (aInt < angIni) aInt += 2.0 * M_PI;
            }
            else
            {
                if (angIni < angFim) angIni += 2.0 * M_PI;
                if (aInt < angFim) aInt += 2.0 * M_PI;
            }
            bool noArco = (bulge > 0) ? (aInt <= angFim) : (aInt <= angIni);
            if (noArco)
            {
                double zInt = a1.z + t * (a2.z - a1.z);
                double deltaAng = std::abs(aInt - angIni);
                double comprimentoAtePonto = raio * deltaAng;
                PontoIntersecaoTIN ponto;
                ponto.global = Eigen::Vector3d(pInt.x(), pInt.y(), zInt);
                ponto.cota = zInt;
                ponto.estaca = estacaInicial + comprimentoAtePonto;
                ponto.offset = 0.0;
                resultados.push_back(ponto);
            }
        }
    }
    return resultados;
}

std::vector<PontoIntersecaoTIN> SegmentoHorizontal::interceptarReta(const Ponto& a1, const Ponto& a2) const
{
    std::vector<PontoIntersecaoTIN> resultados;
    double x1 = pIni.x, y1 = pIni.y;
    double x2 = pFim.x, y2 = pFim.y;
    double x3 = a1.x,   y3 = a1.y;
    double x4 = a2.x,   y4 = a2.y;
    double den = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
    if (std::abs(den) < 1e-9) return resultados;
    double t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / den;
    double u = -((x1 - x2) * (y1 - y3) - (y1 - y2) * (x1 - x3)) / den;
    if (t >= 0.0 && t <= 1.0 && u >= 0.0 && u <= 1.0)
    {
        double zInt = a1.z + u * (a2.z - a1.z);
        double px = x1 + t * (x2 - x1);
        double py = y1 + t * (y2 - y1);
        PontoIntersecaoTIN ponto;
        ponto.global = Eigen::Vector3d(px, py, zInt);
        ponto.cota = zInt;
        ponto.estaca = estacaInicial + (t * comprimento);
        ponto.offset = 0.0;
        resultados.push_back(ponto);
    }
    return resultados;
}

Eigen::Vector2d SegmentoHorizontal::calcularPerpendicularLocal(double s) const
{
    if (tipo == TipoElemento::Reta)
    {
        Eigen::Vector2d vDir = (Eigen::Vector2d(pFim.x, pFim.y) - Eigen::Vector2d(pIni.x, pIni.y)).normalized();
        return Eigen::Vector2d(vDir.y(), -vDir.x());
    }
    else
    {
        Eigen::Vector2d centro = calcularCentro();
        Eigen::Vector2d pEstaca = getXYNaEstaca(s);
        Eigen::Vector2d radial = (pEstaca - centro).normalized();
        return (bulge > 0) ? radial : -radial;
    }
}

Eigen::Vector2d SegmentoHorizontal::getXYNaEstaca(double s) const
{
    double ds = s - estacaInicial;
    if (tipo == TipoElemento::Reta)
    {
        Eigen::Vector2d dir = (Eigen::Vector2d(pFim.x, pFim.y) -
                               Eigen::Vector2d(pIni.x, pIni.y)).normalized();
        return Eigen::Vector2d(pIni.x, pIni.y) + dir * ds;
    }
    else
    {
        Eigen::Vector2d centro = calcularCentro();
        double angIni = atan2(pIni.y - centro.y(), pIni.x - centro.x());
        double deltaAng = ds / raio;
        double angAtual = (bulge > 0) ? (angIni + deltaAng) : (angIni - deltaAng);
        return centro + Eigen::Vector2d(cos(angAtual), sin(angAtual)) * raio;
    }
}

std::vector<PontoIntersecaoTIN> SegmentoHorizontal::interceptarRetaManual(const Ponto& p1, const Ponto& p2, const Ponto& a1, const Ponto& a2) {
    std::vector<PontoIntersecaoTIN> resultados;

    double x1 = p1.x, y1 = p1.y;
    double x2 = p2.x, y2 = p2.y;
    double x3 = a1.x, y3 = a1.y;
    double x4 = a2.x, y4 = a2.y;

    double den = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
    if (std::abs(den) < 1e-9) return resultados;

    double t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / den;
    double u = -((x1 - x2) * (y1 - y3) - (y1 - y2) * (x1 - x3)) / den;

    if (t >= 0.0 && t <= 1.0 && u >= 0.0 && u <= 1.0) {
        PontoIntersecaoTIN res;
        // Se p1 e p2 forem do eixo, 't' ajuda na estaca.
        // Se forem da seção transversal, 't' ajuda no offset.
        res.cota = a1.z + u * (a2.z - a1.z);
        res.global = Eigen::Vector3d(x1 + t * (x2 - x1), y1 + t * (y2 - y1), res.cota);
        resultados.push_back(res);
    }
    return resultados;
}

Eigen::Vector2d EixoHorizontal::getPerpendicularNaEstaca(double s) const
{
    for (const auto& seg : trechos)
    {
        if (s >= seg.estacaInicial && s <= seg.estacaFinal)
        {
            return seg.calcularPerpendicularLocal(s);
        }
    }
    if (s < estacaPartida) return trechos.front().calcularPerpendicularLocal(s);
    return trechos.back().calcularPerpendicularLocal(s);
}

// --- Implementação do Maestro (Eixo) ---
Eigen::Vector2d EixoHorizontal::getXYNaEstaca(double s) const {
    // 1. Procura o segmento que contém a estaca s
    for (const auto& seg : trechos) {
        if (s >= seg.estacaInicial && s <= (seg.estacaFinal + 1e-4)) { // 1e-4 para tolerância de arredondamento
            return seg.getXYNaEstaca(s); // Delega para a matemática do segmento
        }
    }

    // 2. Tratamento de bordas (extrapolação)
    if (trechos.empty()) return Eigen::Vector2d(0, 0);
    if (s < estacaPartida) return trechos.front().getXYNaEstaca(estacaPartida);

    return trechos.back().getXYNaEstaca(trechos.back().estacaFinal);
}
