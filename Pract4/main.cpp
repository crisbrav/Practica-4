#include "Red.h"
#include <iostream>

using namespace std;

int leerEntero(const string& prompt) {
    cout << prompt;
    int x;
    while (!(cin >> x)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Entrada invalida, intente de nuevo: ";
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return x;
}

string leerCadena(const string& prompt) {
    cout << prompt;
    string s;
    getline(cin, s);
    if (s.empty()) getline(cin, s);
    return s;
}

void mostrarMenu() {
    cout << "\n=== Menu Red de Enrutadores ===\n";
    cout << "1. Cargar red desde archivo\n";
    cout << "2. Generar red aleatoria\n";
    cout << "3. Agregar enrutador\n";
    cout << "4. Eliminar enrutador\n";
    cout << "5. Agregar o actualizar enlace\n";
    cout << "6. Eliminar enlace\n";
    cout << "7. Recalcular tablas (Dijkstra por cada origen)\n";
    cout << "8. Mostrar tabla de un enrutador\n";
    cout << "9. Ruta mas corta entre dos enrutadores\n";
    cout << "10. Listar enrutadores y enlaces\n";
    cout << "0. Salir\n";
}

int main() {
    Red red;

    // Estado inicial opcional: vacio
    bool salir = false;
    while (!salir) {
        mostrarMenu();
        int op = leerEntero("Opcion: ");
        switch (op) {
        case 1: {
            string ruta = leerCadena("Ruta del archivo: ");
            if (red.cargarDesdeArchivo(ruta)) {
                red.recomputarTablas();
                cout << "Red cargada y tablas recalculadas.\n";
            } else {
                cout << "Fallo al cargar archivo.\n";
            }
            break;
        }
        case 2: {
            int n = leerEntero("Cantidad de enrutadores (<=26): ");
            double densidad;
            cout << "Densidad de enlaces (0..1): ";
            while (!(cin >> densidad) || densidad < 0.0 || densidad > 1.0) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Entrada invalida, intente de nuevo: ";
            }
            int cmin = leerEntero("Costo minimo: ");
            int cmax = leerEntero("Costo maximo: ");
            unsigned seed = (unsigned)leerEntero("Semilla (0 para aleatoria): ");
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            red.generarAleatoria(n, densidad, cmin, cmax, seed);
            red.recomputarTablas();
            cout << "Red aleatoria generada y tablas recalculadas.\n";
            break;
        }
        case 3: {
            string id = leerCadena("ID del nuevo enrutador: ");
            if (red.agregarEnrutador(id)) {
                red.recomputarTablas();
                cout << "Enrutador agregado y tablas recalculadas.\n";
            } else {
                cout << "Ya existe ese enrutador.\n";
            }
            break;
        }
        case 4: {
            string id = leerCadena("ID del enrutador a eliminar: ");
            if (red.eliminarEnrutador(id)) {
                red.recomputarTablas();
                cout << "Enrutador eliminado y tablas recalculadas.\n";
            } else {
                cout << "No existe ese enrutador.\n";
            }
            break;
        }
        case 5: {
            string a = leerCadena("Enrutador A: ");
            string b = leerCadena("Enrutador B: ");
            int w = leerEntero("Costo: ");
            int bi = leerEntero("Bidireccional? 1=si 0=no: ");
            if (red.actualizarCosto(a,b,w, bi!=0)) {
                red.recomputarTablas();
                cout << "Enlace agregado/actualizado y tablas recalculadas.\n";
            } else {
                cout << "Fallo: verifique que existan los enrutadores.\n";
            }
            break;
        }
        case 6: {
            string a = leerCadena("Enrutador A: ");
            string b = leerCadena("Enrutador B: ");
            int bi = leerEntero("Bidireccional? 1=si 0=no: ");
            if (red.eliminarEnlace(a,b, bi!=0)) {
                red.recomputarTablas();
                cout << "Enlace eliminado y tablas recalculadas.\n";
            } else {
                cout << "Fallo: verifique que existan los enrutadores.\n";
            }
            break;
        }
        case 7: {
            red.recomputarTablas();
            cout << "Tablas recalculadas.\n";
            break;
        }
        case 8: {
            string o = leerCadena("Origen: ");
            red.imprimirTabla(o);
            break;
        }
        case 9: {
            string o = leerCadena("Origen: ");
            string d = leerCadena("Destino: ");
            auto ans = red.rutaMasCorta(o,d);
            if (ans.first < 0) {
                cout << "No hay ruta de " << o << " a " << d << "\n";
            } else {
                cout << "Costo " << ans.first << " | Camino: ";
                for (size_t i=0;i<ans.second.size();++i){
                    if (i) cout << " -> ";
                    cout << ans.second[i];
                }
                cout << "\n";
            }
            break;
        }
        case 10: {
            red.listarRed();
            break;
        }
        case 0: {
            salir = true;
            break;
        }
        default:
            cout << "Opcion invalida.\n";
        }
    }
    return 0;
}
