#include "Enrutador.h"
#include <limits>
#include <algorithm>

using namespace std;

Enrutador::Enrutador(const string& nombre)
    : id(nombre), distancia(numeric_limits<int>::max()), visitado(false), previo(nullptr) {}

void Enrutador::nuevoVecino(Enrutador* v, int costo) {
    // Si existe, actualiza costo; si no, agrega
    for (auto& e : vecinos) {
        if (e.vecino == v) { e.costo = costo; return; }
    }
    vecinos.emplace_back(v, costo);
}

void Enrutador::eliminarVecino(const string& nombreVecino) {
    vecinos.erase(remove_if(vecinos.begin(), vecinos.end(), [&](const Enlace& e){ return e.vecino && e.vecino->id == nombreVecino; }),
                  vecinos.end());
}

void Enrutador::reinicio() {
    distancia = numeric_limits<int>::max();
    visitado = false;
    previo = nullptr;
}
