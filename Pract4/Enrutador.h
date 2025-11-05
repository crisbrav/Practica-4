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
    string id;                            // nombre del enrutador
    vector<Enlace> vecinos;               // enlaces directos
    int distancia;                        // distancia temporal desde la fuente
    bool visitado;                        // marca de visitado
    Enrutador* previo;                    // predecesor para reconstruir camino

    unordered_map<string,int> tablaCostos;                  // destino -> costo minimo
    unordered_map<string, vector<string>> caminosCompletos; // destino -> camino completo

    explicit Enrutador(const string& nombre);

    void nuevoVecino(Enrutador* v, int costo);
    void eliminarVecino(const string& nombreVecino);
    void reinicio();
};

#endif // ENRUTADOR_H

