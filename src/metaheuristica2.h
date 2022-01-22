#ifndef METAHEURISTICA2_H
#define METAHEURISTICA2_H

#include "auxiliares.h"
#include "heuristica1.h"
#include "heuristica2.h"

using namespace std;

void inicializarMemoria(int k);

void inicializarColoresPorNodo(vector<int> &colores, int n, Grafo &G, int maximoColoraUsar);

void actualizarColoresPorNodo(Grafo &G, int mejor_color, int color_anterior, int mejor_nodo,
                              vector<int> &colores);

void vecindadSwap(Grafo &G, Grafo &H_Lista, int n, vector<int> &colores);

void
vecindadChangeColor(vector<int> &colores, int n, Grafo &G, Grafo &H_Lista, int &menorColorNoUsado);

void metaheuristica2(bool heuristica, int max_it, int max_it_sin_mejora,
                     int max_cant_colores, int tipo_vecindad, int memory_size,
                     int vecindad_size, vector<int> &colores, Grafo &G,
                     Grafo &G_Matriz, Grafo &H_Lista,
                     vector <tuple<int, int, int>> &aristasH, int mG,
                     vector<int> &impactoVsIteraciones);

#endif
