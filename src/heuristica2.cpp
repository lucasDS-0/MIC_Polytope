#include "heuristica2.h"

void ordenarAristasH(vector <tuple<int, int, int>> &aristasH, int mG, int n) {
    //Ordenar las aristas de H según la suma de los grados de sus extremos en G
    //BucketSort (la suma de los grados es a lo sumo MG+1, entonces la cant de buckets es min{m_G+2,2n-2+1}) Es O(mG+mH)
    vector < vector < tuple < int, int, int >> > buckets(
            min(mG + 2, 2 * n - 1),
            vector < tuple < int, int, int >> ()
    );
//Antes decía buckets[get<2>(aristasH[i])-1].push_back(aristasH[i]); no sé por qué le restaba 1
    for (int i = 0; i < aristasH.size(); ++i) {
        buckets[get<2>(aristasH[i])].push_back(aristasH[i]);
    }

    aristasH.clear();
    for (int i = buckets.size() - 1; i >= 0; --i) {
        for (int j = 0; j < buckets[i].size(); ++j) {
            aristasH.push_back(buckets[i][j]);
        }
    }
}


void heuristicaG(vector<int> &colores, Grafo &G, Grafo &H, int maximoColorUsado) {
    for (int i = 0; i < G.size(); ++i) {
        if (colores[i] == UNDEFINED) {
            vector<bool> coloresVecinos(maximoColorUsado + 1, false);
            for (int j = 0; j < G[i].size(); ++j) {
                int colorVecino = colores[G[i][j]];
                if (colorVecino != UNDEFINED) {
                    coloresVecinos[colorVecino] = true;
                }
            }
            int j = 0;
            while (j < coloresVecinos.size() && coloresVecinos[j]) {
                j++;
            }
            colores[i] = j;
            maximoColorUsado = max(maximoColorUsado, j);
        }
    }
}


void heuristica2(vector<int> &colores, Grafo &G_Lista, Grafo &G_Matriz, Grafo &H,
                 vector <tuple<int, int, int>> &aristasH, int mG, int n) {
    ordenarAristasH(aristasH, mG, n);
    int maximoColorActual = 0;
    for (int i = 0; i < aristasH.size(); ++i) {
        //Extremos de la arista
        int u = get<0>(aristasH[i]);
        int v = get<1>(aristasH[i]);


        if (!G_Matriz[u][v]) {
            if (colores[u] == UNDEFINED &&
                colores[v] == UNDEFINED) {// ambos extremos aún sin pintar
                //Acá se guardará la información de qué colores están prohibidos para los extremos de la arista
                vector<bool> coloresVecinos(maximoColorActual + 1, false);
                for (int j = 0; j < G_Lista[u].size(); ++j) {
                    int colorVecino = colores[G_Lista[u][j]];
                    if (colorVecino != UNDEFINED) {
                        coloresVecinos[colorVecino] = true;
                    }
                }
                for (int j = 0; j < G_Lista[v].size(); ++j) {
                    int colorVecino = colores[G_Lista[v][j]];
                    if (colorVecino != UNDEFINED) {
                        coloresVecinos[colorVecino] = true;
                    }
                }
                int j = 0;
                while (j < coloresVecinos.size() && coloresVecinos[j]) {
                    j++;
                }
                colores[u] = j;
                colores[v] = j;
                maximoColorActual = max(maximoColorActual, j);
            } else if (colores[u] == UNDEFINED) { //el extremo u sin pintar y v pintado
                //Chequeamos si u se puede pintar del color que ya tiene v
                bool pintable = true;
                for (int j = 0; j < G_Lista[u].size(); ++j) {
                    if (colores[G_Lista[u][j]] == colores[v]) {
                        pintable = false;
                        break;
                    }
                }
                if (pintable) colores[u] = colores[v];
            } else if (colores[v] == UNDEFINED) { //el extremo v sin pintar y u pintado
                bool pintable = true;
                for (int j = 0; j < G_Lista[v].size(); ++j) {
                    if (colores[G_Lista[v][j]] == colores[u]) {
                        pintable = false;
                        break;
                    }
                }
                if (pintable) colores[v] = colores[u];
            }
        }
    }

    //Colorear los nodos faltantes
    heuristicaG(colores, G_Lista, H, maximoColorActual);
}
