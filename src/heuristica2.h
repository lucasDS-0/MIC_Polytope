#ifndef HEURISTICA_H
#define HEURISTICA_H

#include "auxiliares.h"

using namespace std;

void ordenarAristasH(vector<tuple<int, int, int>> &aristasH, int mG, int n);

void heuristicaG(vector<int> &colores,  Grafo &G,  Grafo &H, int maximoColorUsado = 0);

void heuristica2(vector<int> &colores,  Grafo &G_Lista, Grafo &G_Matriz,  Grafo &H,vector <tuple<int, int, int>> &aristasH, int mG, int n);

#endif
