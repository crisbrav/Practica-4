#include "Red.h"
#include <limits>

using namespace std;

bool Red::agregarEnrutador(const string& id) {
    if (routers.count(id)) return false;
    routers[id] = make_unique<Enrutador>(id);
    return true;
}

bool Red::agregarEnlace(const string& a, const string& b, int costo, bool bidir) {
    if (!routers.count(a) || !routers.count(b)) return false;
    auto* A = routers[a].get();
    auto* B = routers[b].get();
    A->nuevoVecino(B, costo);
    if (bidir) B->nuevoVecino(A, costo);
    return true;
}

void Red::recomputarTablas() {
    for (auto& kv : routers) dijkstra(kv.second.get());
}

void Red::dijkstra(Enrutador* fuente) {
    for (auto& kv : routers) kv.second->reinicio();
    fuente->distancia = 0;

    priority_queue<PQN, vector<PQN>, Cmp> pq;
    pq.push({0, fuente});

    while (!pq.empty()) {
        auto cur = pq.top(); pq.pop();
        Enrutador* u = cur.p;
        if (u->visitado) continue;
        u->visitado = true;

        for (auto& e : u->vecinos) {
            Enrutador* v = e.vecino;
            int nd = u->distancia + e.costo;
            if (nd < v->distancia) {
                v->distancia = nd;
                v->previo = u;
                pq.push({nd, v});
            }
        }
    }

    fuente->tablaCostos.clear();
    fuente->caminosCompletos.clear();
    for (auto& kv : routers) {
        Enrutador* dest = kv.second.get();
        if (dest->distancia == numeric_limits<int>::max()) continue;
        vector<string> path = reconstruirCamino(dest);
        fuente->tablaCostos[dest->id] = dest->distancia;
        fuente->caminosCompletos[dest->id] = move(path);
    }
}

vector<string> Red::reconstruirCamino(Enrutador* destino) const {
    vector<string> camino;
    for (Enrutador* cur = destino; cur; cur = cur->previo) camino.push_back(cur->id);
    reverse(camino.begin(), camino.end());
    return camino;
}

pair<int, vector<string>> Red::rutaMasCorta(const string& origen, const string& destino) const {
    if (!routers.count(origen) || !routers.count(destino)) return {-1, {}};
    const Enrutador* o = routers.at(origen).get();
    auto itC = o->tablaCostos.find(destino);
    auto itP = o->caminosCompletos.find(destino);
    if (itC == o->tablaCostos.end() || itP == o->caminosCompletos.end()) return {-1, {}};
    return {itC->second, itP->second};
}
