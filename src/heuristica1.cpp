#include "heuristica1.h"

int prox_color_gl = 0;

void preprocesar(Grafo &G_matriz, Grafo &H) {
    Grafo H_pp(H.size(), vector<int>());
    for (int i = 0; i < H.size(); i++) {
        for (int j = 0; j < H[i].size(); j++) {
            if (G_matriz[i][H[i][j]] == 0) {
                H_pp[i].push_back(H[i][j]);
            }
        }
    }
    H = H_pp;
}

void ordenarNodosPorGrado(Grafo &H, vector<int> &nodos_ordenados) {
    int n = H.size();
    vector <vector<int>> buckets(n, vector<int>());

    for (int i = 0; i < n; ++i) {
        buckets[H[i].size()].push_back(i);
    }
    int it = n - 1;
    for (int i = 0; i < buckets.size(); i++) {
        for (int j = 0; j < buckets[i].size(); j++) {
            nodos_ordenados[it] = buckets[i][j];
            it--;
        }
    }
}

bool color_vecino_valido(int color, int nodo, int vecino, Grafo &G_lista, vector<int> &coloreo) {
    int color_valido = true;
    for (int i = 0; i < G_lista[nodo].size(); i++) {
        color_valido &= !(coloreo[G_lista[nodo][i]] == color && G_lista[nodo][i] != vecino);
    }
    return color_valido;
}

void nuevo_color(int nodo, vector<int> &coloreo, Grafo &G_lista) {
    int c = 0;
    coloreo[nodo] = c;
    while (!valido_undef(coloreo, G_lista)) {
        c++;
        coloreo[nodo] = c;
    }
}

void heuristica1(vector<int> &coloreo, Grafo &G_lista, Grafo &G_matriz, Grafo &H_lista) {
    // Preprocesamiento de H
    preprocesar(G_matriz, H_lista);
    // Ordenamiento de los nodos por grado en H (>)
    vector<int> nodos_ordenados(H_lista.size(), -1);
    ordenarNodosPorGrado(H_lista, nodos_ordenados);
    // Ciclo principal
    for (int i = 0; i < G_lista.size(); i++) {
        // Nodo de la iteracion
        int actual = nodos_ordenados[i];
        // No está pintado
        if (coloreo[actual] == UNDEFINED) {
            vector<int> vecinos_con_colores = {};
            // Recorro los vecinos coloreados del nodo actual en H
            for (int j = 0; j < H_lista[actual].size(); j++) {
                if (coloreo[H_lista[actual][j]] != UNDEFINED) {
                    vecinos_con_colores.push_back(H_lista[actual][j]);
                }
            }
            // Se ve si se puede pintar de un color de algun vecino en H
            for (int j = 0; j < vecinos_con_colores.size(); j++) {
                int c = coloreo[vecinos_con_colores[j]];
                if (color_vecino_valido(c, actual, vecinos_con_colores[j], G_lista, coloreo)) {
                    coloreo[actual] = c;
                    j = vecinos_con_colores.size();
                }
            }
            // Checkeo si no se pudo pintar de ningun color vecino o no tenia vecinos coloreados
            if (coloreo[actual] == UNDEFINED) {
                // Se pinta de un color nuevo
                nuevo_color(actual, coloreo, G_lista);
            }
            // Se ve si los vecinos del nodo actual en H se pueden colorear del mismo color
            // Si no se puede, no se los colorea
            for (int vecino : H_lista[actual]) {
                if (color_vecino_valido(coloreo[actual], vecino, actual, G_lista, coloreo)) {
                    coloreo[vecino] = coloreo[actual];
                }
            }
        }
        // Esta coloreado o recien fue coloreado
        // Se ve si los vecinos del nodo actual en H se pueden colorear del mismo color
        // Si no se puede, no se los colorea
        for (int vecino : H_lista[actual]) {
            if (color_vecino_valido(coloreo[actual], vecino, actual, G_lista, coloreo)) {
                coloreo[vecino] = coloreo[actual];
            }
        }
    }
}
































