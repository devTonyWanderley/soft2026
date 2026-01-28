#include "Topologia.h"
#include <cmath>
#include <set>
#include <list>

#include <QDebug>

Eigen::Vector3d Face::calcularNormal(const std::vector<Ponto>& pontos) const {
    // v[0], v[1] e v[2] são os índices dos 3 pontos do triângulo
    Eigen::Vector3d v1 = pontos[v[1]].pos3d() - pontos[v[0]].pos3d();
    Eigen::Vector3d v2 = pontos[v[2]].pos3d() - pontos[v[0]].pos3d();
    return v1.cross(v2).normalized();
}

double Superficie::interpolarZ(int i1, int i2, double x, double y) const {
    const Ponto& p1 = pontos[i1];
    const Ponto& p2 = pontos[i2];

    double dTotal = std::sqrt(std::pow(p2.x - p1.x, 2) + std::pow(p2.y - p1.y, 2));
    if (dTotal < 1e-6) return p1.z;

    double dPonto = std::sqrt(std::pow(x - p1.x, 2) + std::pow(y - p1.y, 2));
    double t = dPonto / dTotal;

    return p1.z + t * (p2.z - p1.z);
}

/*
void Superficie::gerarContornoSequencial() {
    this->indicesContorno.clear();
    if (faces.empty()) return;

    // 1. IDENTIFICAR ARESTAS DE BORDA
    std::map<std::pair<int, int>, int> contador;
    for (const auto& f : faces) {
        // Agora acessamos o array v[3] definido no .h
        for (int i = 0; i < 3; i++) {
            int i1 = std::min(f.v[i], f.v[(i + 1) % 3]);
            int i2 = std::max(f.v[i], f.v[(i + 1) % 3]);
            contador[{i1, i2}]++;
        }
    }

    // ... (restante da lógica de elos e encadeamento segue igual)
}
*/

void Superficie::gerarContornoSequencial()
{
    std::vector<ArestaTIN> arContorno;
    arContorno.clear();

    // 1. IDENTIFICAÇÃO DAS ARESTAS DE CONTORNO (Ocorrência única em faces)
    for(int i = 0; i < arestas.size(); i++)
    {
        int n = 0;
        int j = 0;
        while(j < faces.size() && n < 2)
        {
            // Verifica se os vértices da aresta i estão na face j
            bool vIniOk = (arestas.at(i).iIni == faces.at(j).v[0] ||
                           arestas.at(i).iIni == faces.at(j).v[1] ||
                           arestas.at(i).iIni == faces.at(j).v[2]);
            bool vFimOk = (arestas.at(i).iFim == faces.at(j).v[0] ||
                           arestas.at(i).iFim == faces.at(j).v[1] ||
                           arestas.at(i).iFim == faces.at(j).v[2]);

            if(vIniOk && vFimOk) n++;
            j++;
        }
        if(n == 1) arContorno.push_back(arestas.at(i));
    }

    qDebug() << "Arestas de contorno encontradas:" << arContorno.size();
    if(arContorno.empty()) return;

    // 2. MONTAGEM DA SEQUÊNCIA (Encadeamento)
    iContorno.clear();
    // Inicializa com a primeira aresta encontrada
    iContorno.push_back(arContorno.at(0).iIni);
    iContorno.push_back(arContorno.at(0).iFim);
    arContorno.erase(arContorno.begin()); // Remove a que foi usada (índice 0)

    std::vector<ArestaTIN> aux;
    bool houveEncaixeNestaRodada = false;

    while(!arContorno.empty())
    {
        ArestaTIN atual = arContorno.back();
        arContorno.pop_back();

        bool encaixou = false;

        // Tenta encaixar no início da lista iContorno
        if(atual.iIni == iContorno.front()) {
            iContorno.push_front(atual.iFim);
            encaixou = true;
        }
        else if(atual.iFim == iContorno.front()) {
            iContorno.push_front(atual.iIni);
            encaixou = true;
        }
        // Tenta encaixar no fim da lista iContorno
        else if(atual.iIni == iContorno.back()) {
            iContorno.push_back(atual.iFim);
            encaixou = true;
        }
        else if(atual.iFim == iContorno.back()) {
            iContorno.push_back(atual.iIni);
            encaixou = true;
        }

        if(encaixou) {
            houveEncaixeNestaRodada = true;
        } else {
            aux.push_back(atual);
        }

        // Se arContorno esvaziou, verificamos se podemos continuar
        if(arContorno.empty()) {
            if(!houveEncaixeNestaRodada) {
                // ESPIÃO: Se rodamos tudo e nada encaixou, temos um contorno isolado ou buraco.
                qWarning() << "Aviso: Restaram" << aux.size() << "arestas que não conectam ao contorno principal.";
                break; // Sai do loop para evitar travamento infinito
            }

            // Devolve os que sobraram para tentar nova rodada
            arContorno = aux;
            aux.clear();
            houveEncaixeNestaRodada = false;
        }
    }

    qDebug() << "Contorno sequencial finalizado com" << iContorno.size() << "pontos.";
}


void Superficie::reconstruirFaces()
{
    faces.clear();
    int i = 0, n = 0, j, k, p, q, r;
    while(i < (arestas.size() - 2))
    {
        j = i + 1;
        while((j < (arestas.size() - 1)) && (n < 2))
        {
            p = q = r = -1;
            if(arestas.at(i).iIni == arestas.at(j).iIni)
            {
                p = arestas.at(i).iFim;
                q = arestas.at(j).iFim;
                r = arestas.at(j).iIni;
            }
            else
            {
                if(arestas.at(i).iIni == arestas.at(j).iFim)
                {
                    p = arestas.at(i).iFim;
                    q = arestas.at(j).iIni;
                    r = arestas.at(j).iFim;
                }
                else
                {
                    if(arestas.at(i).iFim == arestas.at(j).iFim)
                    {
                        p = arestas.at(i).iIni;
                        q = arestas.at(j).iIni;
                        r = arestas.at(j).iFim;
                    }
                    else
                    {
                        if(arestas.at(i).iFim == arestas.at(j).iIni)
                        {
                            p = arestas.at(i).iIni;
                            q = arestas.at(j).iFim;
                            r = arestas.at(j).iIni;
                        }
                    }
                }
            }
            if((p != -1) && (q != -1))
            {
                k = j + 1;
                bool flg = true;
                while((k < arestas.size()) && flg)
                {
                    if((p == arestas.at(k).iIni && q == arestas.at(k).iFim) ||
                        (p == arestas.at(k).iFim && q == arestas.at(k).iIni))
                    {
                        n++;
                        flg = false;
                        Face f;
                        f.v[0] = p;
                        f.v[1] = q;
                        f.v[2] = r;
                        faces.push_back(f);
                    }
                    k++;
                }
            }
            j++;
        }
        i++;
        n = 0;
    }
}

double Superficie::obterZ(double x, double y) {
    Eigen::Vector2d p(x, y);

    for (const auto& f : faces) {
        Eigen::Vector2d v0 = pontos[f.v[0]].pos2d();
        Eigen::Vector2d v1 = pontos[f.v[1]].pos2d();
        Eigen::Vector2d v2 = pontos[f.v[2]].pos2d();

        // Cálculo de coordenadas baricêntricas
        Eigen::Vector2d v10 = v1 - v0;
        Eigen::Vector2d v20 = v2 - v0;
        Eigen::Vector2d vp0 = p - v0;

        double d00 = v10.dot(v10);
        double d01 = v10.dot(v20);
        double d11 = v20.dot(v20);
        double d20 = vp0.dot(v10);
        double d21 = vp0.dot(v20);
        double denom = d00 * d11 - d01 * d01;

        if (std::abs(denom) < 1e-9) continue;

        double v = (d11 * d20 - d01 * d21) / denom;
        double w = (d00 * d21 - d01 * d20) / denom;
        double u = 1.0 - v - w;

        // Se u, v, w estão entre 0 e 1, o ponto está dentro do triângulo
        if (u >= -0.001 && v >= -0.001 && w >= -0.001) {
            return u * pontos[f.v[0]].z + v * pontos[f.v[1]].z + w * pontos[f.v[2]].z;
        }
    }
    return -999.0; // Ponto fora da TIN
}

void Superficie::processarProjeto(const std::vector<DadosBrutos>& eixo) {
    double estacaAcumulada = 0;

    for (const auto& seg : eixo) {
        // Vetor direção do segmento do eixo
        Eigen::Vector2d pIni = seg.p1.pos2d();
        Eigen::Vector2d pFim = seg.p2.pos2d();
        Eigen::Vector2d dir = (pFim - pIni).normalized();
        Eigen::Vector2d perp(-dir.y(), dir.x()); // Perpendicular à esquerda

        // 1. Calcular Cota no Eixo (Offset 0)
        double zEixo = obterZ(pIni.x(), pIni.y());

        // 2. Gerar Seção (Exemplo: pontos a cada 2 metros até o contorno)
        std::vector<PontoSecao> pontosSecao;

        // Espiões de limite lateral (contorno)
        double distEsq = StorageProvider::calcularDistanciaAoContorno(pIni, perp, *this);
        double distDir = StorageProvider::calcularDistanciaAoContorno(pIni, -perp, *this);

        // Lado Esquerdo (Offsets negativos)
        for (double d = 0; d <= distEsq; d += 2.0) {
            Eigen::Vector2d pos = pIni + (perp * d);
            pontosSecao.push_back({ -d, obterZ(pos.x(), pos.y()) });
        }
        // Lado Direito (Offsets positivos)
        for (double d = 2.0; d <= distDir; d += 2.0) {
            Eigen::Vector2d pos = pIni - (perp * d);
            pontosSecao.push_back({ d, obterZ(pos.x(), pos.y()) });
        }

        // 3. Exportar usando seu StorageProvider
        QString nomeArq = QString("Secao_Estaca_%1.txt").arg(estacaAcumulada);
        StorageProvider::exportarSecaoIndividual(nomeArq, estacaAcumulada, pontosSecao);

        estacaAcumulada += (pFim - pIni).norm();
    }
}
