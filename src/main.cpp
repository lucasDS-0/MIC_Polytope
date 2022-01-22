#include <iostream>
#include <map>
#include <chrono>
#include "auxiliares.h"
#include "heuristica1.h"
#include "heuristica2.h"
#include "metaheuristica2.h"
#include "metaheuristica1.h"

//Declaración de variables globales
// #V, #E_G, #E_H, v, w
int n, mG, mH, v, w;

int impacto;

//Declaración de estructuras
vector <tuple<int, int, int>> aristasH;
using namespace std;

bool _heuristica;
int max_it;
int max_it_sin_mejora;
int max_cant_colores;
int tipo_vecindad;
int memory_size;
int vecindad_size;
int aspiracion;
int num_factor;
vector<int> impactoVsIteraciones;

int main(int argc, char **argv) {
    // Leemos el parametro que indica el algoritmo a ejecutar.
    map <string, string> heuristicas = {
            {"H1",  "Heurística 1"},
            {"H2",  "Heurística 2"},
            {"MH1", "Meta Heurística 1"},
            {"MH2", "Meta Heurística 2"},
    };

    if (argc < 2) {
        cout << "USO: cat <ARCHIVO_INSTANCIA> | ./tp2 <H1 | H2 | MH1 | MH2>" << endl;
        return 1;
    }

    string heuristica = argv[1];

    // Lectura del input.
    if (heuristica == "MH1" || heuristica == "MH2") {
        cin >> _heuristica >> max_it >> max_it_sin_mejora >> max_cant_colores >> tipo_vecindad
            >> memory_size >> vecindad_size >> aspiracion >> num_factor;
    }
    cin >> n >> mG >> mH;

    // Vector de colores
    vector<int> colores(n, UNDEFINED);

    // Creacion.

    Grafo G_Matriz(n, vector<int>(n, 0));
    Grafo H_Matriz(n, vector<int>(n, 0));

    Grafo G_Lista(n, vector<int>());
    Grafo H_Lista(n, vector<int>());

    for (int i = 0; i < mG; i++) {
        cin >> v >> w;
        G_Matriz[v - 1][w - 1] = 1;
        G_Matriz[w - 1][v - 1] = 1;

        G_Lista[v - 1].push_back(w - 1);
        G_Lista[w - 1].push_back(v - 1);
    }

    for (int i = 0; i < mH; i++) {
        cin >> v >> w;
        H_Matriz[v - 1][w - 1] = 1;
        H_Matriz[w - 1][v - 1] = 1;

        H_Lista[v - 1].push_back(w - 1);
        H_Lista[w - 1].push_back(v - 1);

        aristasH.push_back(make_tuple(v - 1, w - 1, G_Lista[v - 1].size() + G_Lista[w - 1].size()));
    }

    // Ejecucion.

    auto start = chrono::steady_clock::now();

    if (heuristica == "H1") {

        heuristica1(colores, G_Lista, G_Matriz, H_Lista);
        impacto = impacto_lista(H_Lista, colores);
        // coloreo_es_valido(colores, G_Lista);
    }

    if (heuristica == "H2") {

        heuristica2(colores, G_Lista, G_Matriz, H_Lista, aristasH, mG, n);
        impacto = impacto_lista(H_Lista, colores);
        // coloreo_es_valido(colores, G_Lista);
    }

    if (heuristica == "MH2") {

        metaheuristica2(_heuristica, max_it, max_it_sin_mejora, max_cant_colores, tipo_vecindad,
                        memory_size, vecindad_size, colores, G_Lista, G_Matriz, H_Lista, aristasH,
                        mG, impactoVsIteraciones);

        impacto = impacto_lista(H_Lista, colores);
    }

    if (heuristica == "MH1") {
        srand(time(0));
        tabuSearch1(colores, G_Lista, G_Matriz, H_Lista, aristasH, mG, _heuristica,
                    max_it, max_it_sin_mejora, max_cant_colores, aspiracion, num_factor);
        impacto = impacto_lista(H_Lista, colores);
    }

    auto end = chrono::steady_clock::now();
    double total_time = chrono::duration<double, milli>(end - start).count();

    string format = "";
    if (argc > 2) {
        format = argv[2];
    }

    if (format == "--json") {
        json_output(n, mG, mH, impacto, total_time, impactoVsIteraciones);
    } else {
        cout << "Impacto: " << impacto << endl;
        cout << "Colores: [ ";

        for (unsigned int i = 0; i < colores.size(); i++) {
            cout << colores[i] << " ";
        }
        cout << "]" << endl;
        clog << impacto << endl;
        clog << total_time << endl;
        for (unsigned int i = 0; i < impactoVsIteraciones.size(); ++i) {
            clog << impactoVsIteraciones[i] << endl;
        }
    }

    return 0;
}
