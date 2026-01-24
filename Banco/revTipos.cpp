#include "revTipos.h"
#include <QDebug>

Eigen::Vector2d SegmentoHorizontal::calcularCentro() const
{
    Eigen::Vector2d p1(pIni.x, pIni.y);
    Eigen::Vector2d p2(pFim.x, pFim.y);

    Eigen::Vector2d vCorda = p2 - p1;
    double dCorda = vCorda.norm();
    Eigen::Vector2d pMedio = p1 + vCorda * 0.5;

    // Evitar divisão por zero se bulge for nulo
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

        // Ângulos de entrada e saída
        double angIni = std::atan2(pIni.y - centro.y(), pIni.x - centro.x());
        double angFim = std::atan2(pFim.y - centro.y(), pFim.x - centro.x());

        // Normalizar ângulos para garantir sentido de varredura
        if (bulge < 0 && angFim > angIni) angIni += 2.0 * M_PI;
        if (bulge > 0 && angIni > angFim) angFim += 2.0 * M_PI;

        double aMin = std::min(angIni, angFim);
        double aMax = std::max(angIni, angFim);

        // Checar se os quadrantes (0, 90, 180, 270) estão dentro do arco
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

    if (delta < 0) return resultados; // Não cruza

    // Bhaskara para encontrar o parâmetro t da aresta
    double t_valores[2] = { (-b + sqrt(delta)) / (2.0 * a), (-b - sqrt(delta)) / (2.0 * a) };

    for (double t : t_valores)
    {
        // 1. O ponto está dentro do segmento da aresta? (t entre 0 e 1)
        if (t >= 0.0 && t <= 1.0)
        {
            Eigen::Vector2d pInt = A + t * V;

            // 2. O ponto está dentro do ARCO (entre PC e PT)?
            // Aqui usamos o ângulo para validar se o ponto não está no "resto" do círculo
            double angInt = atan2(pInt.y() - C.y(), pInt.x() - C.x());

            // 2. O ponto está dentro do ARCO (entre PC e PT)?
            double angIni = atan2(pIni.y - C.y(), pIni.x - C.x());
            double angFim = atan2(pFim.y - C.y(), pFim.x - C.x());
            double aInt = angInt; // Nosso angInt calculado anteriormente

            // Normalização para sentido de varredura (Bulge > 0: Anti-horário, < 0: Horário)
            if (bulge > 0) {
                if (angFim < angIni) angFim += 2.0 * M_PI;
                if (aInt < angIni) aInt += 2.0 * M_PI;
            } else {
                if (angIni < angFim) angIni += 2.0 * M_PI;
                if (aInt < angFim) aInt += 2.0 * M_PI;
            }

            // Verifica se o ângulo está no intervalo do arco
            bool noArco = (bulge > 0) ? (aInt <= angFim) : (aInt <= angIni);

            if (noArco) {
                double zInt = a1.z + t * (a2.z - a1.z);

                // 3. Cálculo da Estaca no Arco: S = S_ini + (Raio * Delta_Angulo)
                double deltaAng = std::abs(aInt - angIni);
                double comprimentoAtePonto = raio * deltaAng;

                PontoIntersecaoTIN ponto;
                ponto.global = Eigen::Vector3d(pInt.x(), pInt.y(), zInt);
                ponto.cota = zInt;
                ponto.estaca = estacaInicial + comprimentoAtePonto;
                ponto.offset = 0.0; // Perfil Longitudinal

                resultados.push_back(ponto);
            }
        }
    }
    return resultados;
}

std::vector<PontoIntersecaoTIN> SegmentoHorizontal::interceptarReta(const Ponto& a1, const Ponto& a2) const
{
    std::vector<PontoIntersecaoTIN> resultados;

    // Vetores diretores
    double x1 = pIni.x, y1 = pIni.y;
    double x2 = pFim.x, y2 = pFim.y;
    double x3 = a1.x,   y3 = a1.y;
    double x4 = a2.x,   y4 = a2.y;

    // Denominador comum (Determinante da matriz de coeficientes)
    double den = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);

    // Se den == 0, as linhas são paralelas
    if (std::abs(den) < 1e-9) return resultados;

    // Cramer para encontrar os parâmetros t (eixo) e u (aresta)
    double t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / den;
    double u = -((x1 - x2) * (y1 - y3) - (y1 - y2) * (x1 - x3)) / den;

    // Verificação: o cruzamento ocorre dentro de ambos os segmentos?
    if (t >= 0.0 && t <= 1.0 && u >= 0.0 && u <= 1.0)
    {
        // Interpolação da Cota Z na aresta
        double zInt = a1.z + u * (a2.z - a1.z);

        // Coordenadas globais
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

void Corredor::gerarPerfilLongitudinal(const Superficie& terreno)
{
    this->vertical.pontos.clear(); // Limpa perfil anterior
    std::vector<PontoIntersecaoTIN> todasIntersecoes;

    // 1. Varre cada segmento do eixo (Reta ou Curva)
    for (const auto& seg : horizontal.trechos)
    {
        BBox boxSeg = seg.calcularBBox();

        // 2. Varre as arestas da TIN
        for (const auto& aresta : terreno.arestas)
        {
            const Ponto& a1 = terreno.pontos[aresta.iIni];
            const Ponto& a2 = terreno.pontos[aresta.iFim];

            // Filtro de Performance AABB
            BBox boxAresta;
            boxAresta.atualizar(a1.x, a1.y);
            boxAresta.atualizar(a2.x, a2.y);
            if (!boxSeg.intercepta(boxAresta)) continue;

            // 3. Interseção Real (Seletor Reta vs Curva)
            std::vector<PontoIntersecaoTIN> pts;
            if (seg.tipo == TipoElemento::Reta)
            {
                pts = seg.interceptarReta(a1, a2);
                // Aqui você chamará a lógica de Cramer (Reta-Reta) ..............................................................
                // pts = seg.interceptarReta(a1, a2);
            }
            else
            {
                pts = seg.interceptarArco(a1, a2);
            }

            // Adiciona os pontos encontrados à lista global
            todasIntersecoes.insert(todasIntersecoes.end(), pts.begin(), pts.end());
        }
    }

    // 4. ORDENAÇÃO: O segredo para o perfil não "voltar para trás"
    std::sort(todasIntersecoes.begin(), todasIntersecoes.end(),
              [](const PontoIntersecaoTIN& a, const PontoIntersecaoTIN& b)
              {
                  return a.estaca < b.estaca;
              });

    // 5. Transferência para a estrutura de Perfil Longitudinal
    for (const auto& it : todasIntersecoes)
    {
        this->vertical.pontos.emplace_back(it.estaca, it.cota);
    }

    qDebug() << "Perfil Gerado com" << vertical.pontos.size() << "pontos de interseção.";
}

// No revTipos.cpp
double PerfilLongitudinal::cotaNaEstaca(double s) const {
    if (pontos.empty()) return 0.0;
    if (s <= pontos.front().estaca) return pontos.front().cota;
    if (s >= pontos.back().estaca) return pontos.back().cota;

    // Busca binária eficiente (O(log n))
    auto it = std::lower_bound(pontos.begin(), pontos.end(), s,
                               [](const PontoPerfil& p, double val) {
                                   return p.estaca < val;
                               });

    if (it == pontos.begin()) return it->cota;

    // Interpolação Linear entre 'prev' e 'it'
    const auto& p2 = *it;
    const auto& p1 = *std::prev(it);

    double t = (s - p1.estaca) / (p2.estaca - p1.estaca);
    return p1.cota + t * (p2.cota - p1.cota);
}

void Corredor::consolidarEstaqueamentoLongitudinal()
{
    std::set<double> estacasMestre;
    // ... (lógica de união das interseções com as estacas de 5m e 10m)

    // 1. Incluir todos os acidentes geográficos já calculados (Interseções TIN)
    for (const auto& p : vertical.pontos)
    {
        estacasMestre.insert(p.estaca);
    }

    // 2. Incluir estacas de projeto (Inteiras e Intermediárias)
    for (const auto& seg : horizontal.trechos)
    {
        estacasMestre.insert(seg.estacaInicial); // Ponto Notável (PC)
        estacasMestre.insert(seg.estacaFinal);   // Ponto Notável (PT)

        double passo = (seg.tipo == TipoElemento::Reta) ? 10.0 : 5.0;
        double s = std::ceil(seg.estacaInicial / passo) * passo;

        while (s < seg.estacaFinal)
        {
            estacasMestre.insert(s);
            s += passo;
        }
    }

    // 3. Gerar o Perfil Final Ordenado e Interpolado
    std::vector<PontoPerfil> perfilFinal;
    for (double s : estacasMestre)
    {
        double z = vertical.cotaNaEstaca(s); // Usa a interpolação linear rápida
        perfilFinal.emplace_back(s, z);
    }
    vertical.pontos = std::move(perfilFinal);
}
