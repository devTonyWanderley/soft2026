#include "revTipos.h"
#include <QDebug>
#include <QFile>

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

QString ExportEngine::formatarValor(double val, int largura)
{
    // Multiplica por 10.000 para décimo de milímetro
    long long valorInt = static_cast<long long>(std::round(val * 10000.0));
    // Formata com zeros à esquerda: %1 = valor, largura, base 10, preenchimento '0'
    return QString("%1").arg(valorInt, largura, 10, QChar('0')).right(largura);
}

QString ExportEngine::formatarTexto(QString txt, int largura)
{
    return txt.leftJustified(largura, ' ').left(largura);
}

bool ExportEngine::salvarFixo(const QString& caminho,
                              const std::vector<std::map<QString, QString>>& dados,
                              const std::vector<ColunaExport>& colunas)
{
    QFile arquivo(caminho);
    if (!arquivo.open(QIODevice::WriteOnly | QIODevice::Text)) return false;

    QTextStream out(&arquivo);
    for (const auto& linha : dados)
    {
        QString linhaFormatada = "";
        for (const auto& col : colunas)
        {
            linhaFormatada += linha.at(col.titulo);
        }
        out << linhaFormatada << "\n";
    }
    return true;
}

void Corredor::exportarDados(const QString& caminho, Camada tipo)
{
    std::vector<std::map<QString, QString>> linhas;

    // Supondo que estamos exportando o Perfil consolidado
    for (const auto& p : vertical.pontos)
    {
        std::map<QString, QString> linha;
        linha["NOME"] = ExportEngine::formatarTexto("ESTACA", 16);
        linha["VALOR"] = ExportEngine::formatarValor(p.estaca, 12);
        linha["NOME2"] = ExportEngine::formatarTexto("COTA_Z", 16);
        linha["VALOR2"] = ExportEngine::formatarValor(p.cota, 12);
        linhas.push_back(linha);
    }

    // Define o layout uma única vez
    std::vector<ColunaExport> layout = {
        {"NOME", 16, false}, {"VALOR", 12, true},
        {"NOME2", 16, false}, {"VALOR2", 12, true}
    };

    ExportEngine::salvarFixo(caminho, linhas, layout);
}

// No revTipos.h / .cpp
void Superficie::gerarContornoSequencial()
{
    // 1. Mapa para contar ocorrências de cada par de pontos (Arestas)
    std::map<std::pair<int, int>, int> contadorArestas;

    for (const auto& f : faces)
    {
        // Para cada face, garantimos uma ordem (v0-v1, v1-v2, v2-v0)
        auto registrar = [&](int a, int b)
        {
            int pMin = std::min(a, b);
            int pMax = std::max(a, b);
            contadorArestas[{pMin, pMax}]++;
        };
        registrar(f.v[0], f.v[1]);
        registrar(f.v[1], f.v[2]);
        registrar(f.v[2], f.v[0]);
    }

    // 2. Extrair apenas arestas que aparecem uma única vez (Borda)
    std::map<int, std::vector<int>> adjContorno;
    for (auto const& [par, count] : contadorArestas)
    {
        if (count == 1)
        {
            adjContorno[par.first].push_back(par.second);
            adjContorno[par.second].push_back(par.first);
        }
    }

    // 3. "Costurar" os índices em ordem sequencial
    this->indicesContorno.clear();
    if (adjContorno.empty()) return;

    int atual = adjContorno.begin()->first;
    int inicio = atual;
    int anterior = -1;

    do
    {
        indicesContorno.push_back(atual);
        const auto& vizinhos = adjContorno[atual];
        int proximo = (vizinhos[0] == anterior && vizinhos.size() > 1) ? vizinhos[1] : vizinhos[0];
        anterior = atual;
        atual = proximo;
    }
    while (atual != inicio && indicesContorno.size() < adjContorno.size());
}

// No revTipos.cpp
Eigen::Vector2d EixoHorizontal::getPerpendicularNaEstaca(double s) const {
    // Busca o segmento que contém a estaca s
    for (const auto& seg : trechos) {
        if (s >= seg.estacaInicial && s <= seg.estacaFinal) {
            return seg.calcularPerpendicularLocal(s);
        }
    }
    // Caso a estaca esteja fora (extrapolação), usa o último ou primeiro trecho
    if (s < estacaPartida) return trechos.front().calcularPerpendicularLocal(s);
    return trechos.back().calcularPerpendicularLocal(s);
}

// No revTipos.cpp
Eigen::Vector2d SegmentoHorizontal::calcularPerpendicularLocal(double s) const
{
    if (tipo == TipoElemento::Reta)
    {
        // Vetor Diretor Unitário
        Eigen::Vector2d vDir = (Eigen::Vector2d(pFim.x, pFim.y) -
                                Eigen::Vector2d(pIni.x, pIni.y)).normalized();
        // Giro 90° para a direita: (x, y) -> (y, -x)
        return Eigen::Vector2d(vDir.y(), -vDir.x());
    }
    else
    {
        // Lógica de Curva: Vetor Radial
        Eigen::Vector2d centro = calcularCentro();
        Eigen::Vector2d pEstaca = getXYNaEstaca(s); // Função de posição no arco
        Eigen::Vector2d radial = (pEstaca - centro).normalized();

        // Se bulge > 0 (curva à esquerda), o radial aponta para a direita (fora)
        // Se bulge < 0 (curva à direita), o radial aponta para a direita (dentro)
        return (bulge > 0) ? radial : -radial;
    }
}

// Necessário para o passo anterior
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
        // Delta ângulo = comprimento / raio
        double deltaAng = ds / raio;
        double angAtual = (bulge > 0) ? (angIni + deltaAng) : (angIni - deltaAng);

        return centro + Eigen::Vector2d(cos(angAtual), sin(angAtual)) * raio;
    }
}

void Corredor::gerarAmostragemTIN(const Superficie& terreno, double larguraBusca) {
    this->secoes.clear();

    for (const auto& pPerfil : vertical.pontos) {
        SecaoTransversal secao(pPerfil.estaca);

        // 1. GEOMETRIA 2D: Posicionamento no Eixo
        Eigen::Vector2d posEixo = horizontal.getXYNaEstaca(pPerfil.estaca);
        Eigen::Vector2d nPerp = horizontal.getPerpendicularNaEstaca(pPerfil.estaca);

        // Define a "Régua Transversal" (pEsq e pDir) - Sintaxe corrigida
        Ponto pEsq("", "", posEixo.x() - nPerp.x() * larguraBusca, posEixo.y() - nPerp.y() * larguraBusca);
        Ponto pDir("", "", posEixo.x() + nPerp.x() * larguraBusca, posEixo.y() + nPerp.y() * larguraBusca);

        // 2. 3ª COORDENADA: Centro do Eixo com Cota Z do Perfil
        secao.centroEixo = Ponto("", "", posEixo.x(), posEixo.y(), pPerfil.cota);

        // 3. CRUZAMENTO COM A MALHA (Cramer)
        for (const auto& aresta : terreno.arestas) {
            auto inters = SegmentoHorizontal::interceptarRetaManual(
                pEsq, pDir, terreno.pontos[aresta.iIni], terreno.pontos[aresta.iFim]
                );

            for (const auto& pt : inters) {
                Eigen::Vector2d vInt(pt.global.x(), pt.global.y());
                // Produto Escalar para converter Global -> Offset Local
                double offset = (vInt - posEixo).dot(nPerp);
                secao.terreno.emplace_back(offset, pt.cota, "TIN");
            }
        }

        // 4. AJUSTE DO CONTAINER: Ordenação (Esquerda -> Direita)
        std::sort(secao.terreno.begin(), secao.terreno.end(), [](const PontoSecao& a, const PontoSecao& b) {
            return a.offset < b.offset;
        });

        this->secoes.push_back(secao);
    }
}
std::vector<PontoIntersecaoTIN> SegmentoHorizontal::interceptarRetaManual(const Ponto& p1, const Ponto& p2,
                                                                          const Ponto& a1, const Ponto& a2) {
    std::vector<PontoIntersecaoTIN> resultados;

    double x1 = p1.x, y1 = p1.y;
    double x2 = p2.x, y2 = p2.y;
    double x3 = a1.x, y3 = a1.y;
    double x4 = a2.x, y4 = a2.y;

    double den = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
    if (std::abs(den) < 1e-9) return resultados; // Paralelas

    double t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / den;
    double u = -((x1 - x2) * (y1 - y3) - (y1 - y2) * (x1 - x3)) / den;

    if (t >= 0.0 && t <= 1.0 && u >= 0.0 && u <= 1.0) {
        double zInt = a1.z + u * (a2.z - a1.z);
        PontoIntersecaoTIN pt;
        pt.global = Eigen::Vector3d(x1 + t * (x2 - x1), y1 + t * (y2 - y1), zInt);
        pt.cota = zInt;
        resultados.push_back(pt);
    }
    return resultados;
}

Eigen::Vector2d EixoHorizontal::getXYNaEstaca(double s) const {
    // 1. Percorre os trechos do eixo
    for (const auto& seg : trechos) {
        // 2. Verifica se a estaca 's' pertence a este intervalo
        if (s >= seg.estacaInicial && s <= seg.estacaFinal) {
            return seg.getXYNaEstaca(s); // Chama a matemática do segmento
        }
    }

    // 3. Caso de segurança: estaca fora dos limites (extrapolação)
    if (trechos.empty()) return Eigen::Vector2d(0, 0);
    if (s < estacaPartida) return trechos.front().getXYNaEstaca(trechos.front().estacaInicial);
    return trechos.back().getXYNaEstaca(trechos.back().estacaFinal);
}
