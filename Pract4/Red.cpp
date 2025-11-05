#include "Red.h"
#include <limits>

using namespace std;

bool Red::agregarEnrutador(const string& id) {
    if (routers.count(id)) return false;
    routers[id] = make_unique<Enrutador>(id);
    return true;
}

bool Red::eliminarEnrutador(const string& id) {
    if (!routers.count(id)) return false;
    for (auto& kv : routers) kv.second->eliminarVecino(id);
    routers.erase(id);
    return true;
}

bool Red::agregarEnlace(const string& a, const string& b, int costo, bool bidir) {
    if (!routers.count(a) || !routers.count(b)) return false;
    routers[a]->nuevoVecino(routers[b].get(), costo);
    if (bidir) routers[b]->nuevoVecino(routers[a].get(), costo);
    return true;
}

bool Red::eliminarEnlace(const string& a, const string& b, bool bidir) {
    if (!routers.count(a) || !routers.count(b)) return false;
    routers[a]->eliminarVecino(b);
    if (bidir) routers[b]->eliminarVecino(a);
    return true;
}

bool Red::actualizarCosto(const string& a, const string& b, int costo, bool bidir) {
    if (!routers.count(a) || !routers.count(b)) return false;
    return agregarEnlace(a, b, costo, bidir);
}

static bool leerLineaValida(ifstream& in, string& out) {
    string s;
    while (std::getline(in, s)) {
        auto pos = s.find('#');
        if (pos != string::npos) s = s.substr(0,pos);
        auto notsp = s.find_first_not_of(" \t\r\n");
        if (notsp == string::npos) continue;
        auto last = s.find_last_not_of(" \t\r\n");
        out = s.substr(notsp, last - notsp + 1);
        if (!out.empty()) return true;
    }
    return false;
}

bool Red::cargarDesdeArchivo(const string& ruta) {
    ifstream in(ruta);
    if (!in) return false;

    routers.clear();

    string line;
    if (!leerLineaValida(in, line)) return false;
    {
        stringstream ss(line);
        int N=0, M=0;
        if (!(ss >> N >> M)) return false;

        vector<string> nombres;
        while ((int)nombres.size() < N) {
            if (!leerLineaValida(in, line)) return false;
            stringstream sn(line);
            string tok;
            while (sn >> tok) {
                nombres.push_back(tok);
                if ((int)nombres.size() == N) break;
            }
        }
        for (auto& name : nombres) agregarEnrutador(name);

        int leidas = 0;
        while (leidas < M && leerLineaValida(in, line)) {
            stringstream se(line);
            string u, v; int w;
            if (!(se >> u >> v >> w)) return false;
            agregarEnlace(u, v, w, true);
            ++leidas;
        }
        if (leidas != M) return false;
    }
    return true;
}

void Red::generarAleatoria(int n, double densidad, int cmin, int cmax, unsigned seed) {
    routers.clear();
    for (int i=0;i<n;++i) {
        string id(1, char('A'+i));
        agregarEnrutador(id);
    }
    mt19937 rng(seed ? seed : random_device{}());
    uniform_real_distribution<double> prob(0.0,1.0);
    uniform_int_distribution<int> peso(cmin, cmax);
    for (int i=0;i<n;++i){
        for (int j=i+1;j<n;++j){
            if (prob(rng) <= densidad) {
                string a(1, char('A'+i));
                string b(1, char('A'+j));
                agregarEnlace(a,b,peso(rng),true);
            }
        }
    }
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
