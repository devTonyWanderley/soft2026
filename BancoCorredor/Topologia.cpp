#include "Topologia.h"
#include <map>

void Superficie::gerarContornoSequencial()
{
    std::map<std::pair<int, int>, int> contadorArestas;
    for (const auto& f : faces)
    {
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
    std::map<int, std::vector<int>> adjContorno;
    for (auto const& [par, count] : contadorArestas)
    {
        if (count == 1)
        {
            adjContorno[par.first].push_back(par.second);
            adjContorno[par.second].push_back(par.first);
        }
    }
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
