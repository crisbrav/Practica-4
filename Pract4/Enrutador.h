#ifndef ENRUTADOR_H
#define ENRUTADOR_H

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

using namespace std;

class Enrutador;

struct Enlace {
    Enrutador* vecino;
    int costo;
    Enlace(Enrutador* v=nullptr, int c=0): vecino(v), costo(c) {}
};

class Enrutador {
public:
    string id;                            // Nombre del router (A, B, C, etc.)
    vector<Enlace> vecinos;               // Enlaces directos
    int distancia;                        // Distancia temporal desde fuente (para Dijkstra)
    bool visitado;                        // Marca de visitado (para Dijkstra)
    Enrutador* previo;                    // Predecesor en el camino mas corto (para reconstruccion)

    // Tablas finales: costo y camino completo a cada destino
    unordered_map<string,int> tablaCostos;                  // destino -> costo minimo
    unordered_map<string, vector<string>> caminosCompletos; // destino -> camino completo

    explicit Enrutador(const string& nombre);

    void nuevoVecino(Enrutador* v, int costo);
    void eliminarVecino(const string& nombreVecino);
    void reinicio();
};

#endif // ENRUTADOR_H
