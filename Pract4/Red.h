#ifndef RED_H
#define RED_H

#include "Enrutador.h"
#include <memory>
#include <queue>
#include <unordered_map>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

class Red {
public:
    unordered_map<string, unique_ptr<Enrutador>> routers;

    bool agregarEnrutador(const string& id);
    bool agregarEnlace(const string& a, const string& b, int costo, bool bidir=true);

    void recomputarTablas();
    pair<int, vector<string>> rutaMasCorta(const string& origen, const string& destino) const;

private:
    struct PQN { int d; Enrutador* p; };
    struct Cmp { bool operator()(const PQN& a, const PQN& b) const { return a.d > b.d; } };

    void dijkstra(Enrutador* fuente);
    vector<string> reconstruirCamino(Enrutador* destino) const;
};

#endif // RED_H
