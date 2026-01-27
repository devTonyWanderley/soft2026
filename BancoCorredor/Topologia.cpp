#include "Topologia.h"
#include <cmath>
#include <set>

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

void Superficie::gerarContornoSequencial() {
    this->indicesContorno.clear();
    if (arestas.empty()) return;

    // 1. Mapa de Adjacência: Ponto -> Vizinhos
    std::map<int, std::vector<int>> adj;
    for (const auto& ar : arestas) {
        adj[ar.iIni].push_back(ar.iFim);
        adj[ar.iFim].push_back(ar.iIni);
    }

    // 2. Ponto de partida: Menor X (Garantidamente borda)
    int inicio = -1;
    double minX = 1e15;
    for (int i = 0; i < (int)pontos.size(); ++i) {
        if (pontos[i].x < minX) { minX = pontos[i].x; inicio = i; }
    }

    // 3. Caminhada Côncava
    int atual = inicio;
    int anterior = -1;
    Eigen::Vector2d dirRef(0, -1); // Referência inicial para o primeiro ponto

    do {
        indicesContorno.push_back(atual);
        const auto& vizinhos = adj[atual];
        int proximo = -1;
        double maiorAngulo = -1e15;

        for (int v : vizinhos) {
            if (v == anterior) continue;

            Eigen::Vector2d dirAtual = (pontos[v].pos2d() - pontos[atual].pos2d()).normalized();

            // Ângulo relativo à direção de entrada
            double ang = std::atan2(dirRef.x() * dirAtual.y() - dirRef.y() * dirAtual.x(),
                                    dirRef.x() * dirAtual.x() + dirRef.y() * dirAtual.y());

            // Queremos a aresta mais "à direita" para contornar no sentido horário
            if (ang > maiorAngulo) {
                maiorAngulo = ang;
                proximo = v;
            }
        }

        if (proximo == -1) break;

        // Atualiza referência: a nova direção é o oposto da direção que entramos no próximo ponto
        dirRef = (pontos[proximo].pos2d() - pontos[atual].pos2d()).normalized();
        anterior = atual;
        atual = proximo;

    } while (atual != inicio && indicesContorno.size() < pontos.size());
}
