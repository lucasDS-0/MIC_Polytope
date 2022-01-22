#ifndef HEURISTICA1_H
#define HEURISTICA1_H

#include "auxiliares.h"

using namespace std;

void preprocesar(Grafo &G, Grafo &H);

void ordenaNodosPorGrado(Grafo &H, vector<int> &nodos_ordenados);

bool color_vecino_valido(int color, int nodo, int vecino, Grafo &G_lista, vector<int> &coloreo);

void nuevo_color(int nodo, vector<int> &coloreo, Grafo &G_lista);

void heuristica1(vector<int> &colores, Grafo &G_Lista, Grafo &G_matriz, Grafo &H);

#endif
