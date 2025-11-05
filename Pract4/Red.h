#ifndef RED_H
#define RED_H

#include "Enrutador.h"
#include <memory>
#include <queue>
#include <unordered_map>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <random>
#include <algorithm>

using namespace std;

class Red {
public:
    unordered_map<string, unique_ptr<Enrutador>> routers;

    bool agregarEnrutador(const string& id);
    bool eliminarEnrutador(const string& id);

    bool agregarEnlace(const string& a, const string& b, int costo, bool bidir=true);
    bool eliminarEnlace(const string& a, const string& b, bool bidir=true);
    bool actualizarCosto(const string& a, const string& b, int costo, bool bidir=true);

    bool cargarDesdeArchivo(const string& ruta);   // N M | N nombres | M lineas u v w
    void generarAleatoria(int n, double densidad, int cmin, int cmax, unsigned seed=0);

    void recomputarTablas();
    pair<int, vector<string>> rutaMasCorta(const string& origen, const string& destino) const;

private:
    struct PQN { int d; Enrutador* p; };
    struct Cmp { bool operator()(const PQN& a, const PQN& b) const { return a.d > b.d; } };

    void dijkstra(Enrutador* fuente);
    vector<string> reconstruirCamino(Enrutador* destino) const;
};

#endif // RED_H
