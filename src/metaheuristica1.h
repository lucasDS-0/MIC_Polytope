#ifndef METAHEURISTICA1_H
#define METAHEURISTICA1_H

#include "auxiliares.h"
#include "heuristica1.h"
#include "heuristica2.h"
#include "string_map.h"

using namespace std;

void tabuSearch1(vector<int> &vColores, Grafo &G_Lista, Grafo &G_Matriz, Grafo &H_Lista, 
                 vector <tuple<int, int, int>> &aristasH, int mG, bool heuristica, int max_it, 
                 int max_it_sin_mejora, int max_cant_colores, int aspiracion, int num_factor);

#endif
