#include "heuristica2.h"
#include "metaheuristica2.h"
#include <iostream>
#include <cmath>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
using namespace std;
//-----------------------Variables globales------------------------
//Declaración de estructuras
queue<int,list<int>> memoria;
vector<bool> nodos_prohibidos;

//Estructura que por cada nodo, guarda un vector de enteros que indica el número de vecinos de cada nodo, pintado de cada color
vector<vector <int> > colores_por_nodo;

//Variables globales de soluciones óptimas hasta el momento 
int mejorImpactoActual = -1;
vector<int> mejorSolucionActual;
int impactoIteracionActual = 0;

//--------------------Funciones auxiliares--------------------------
//Determina el mayor color usado en una coloración:
int maximoColorUsado (vector<int>& colores){
    int max = 0;
    for (int i = 0; i <colores.size() ; ++i) {
        if (colores[i]>max){
            max = colores[i];
        }
    }
    return max;
}

//Inicializa la memoria de estructura
void inicializarMemoria(int k){
    for (int i = 0; i < k; ++i) {
        memoria.push(-1);
    }
}

//Inicializa el conteo de la cantidad de vecinos de cada color
void inicializarColoresPorNodo(vector<int>& colores, int n, Grafo &G, int maximoColoraUsar){
    vector<bool> vacio;
    colores_por_nodo = vector<vector<int> >(n, vector<int>(maximoColoraUsar,0));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < G[i].size(); ++j) {
            int colorVecino = colores[G[i][j]];
            colores_por_nodo[i][colorVecino]++;
        }
    }
}

//Función para actualizar la cantidad de vecinos de cada color en caso de haber usado vecindadChangeColor (se modificó el color de un solo nodo)
void actualizarColoresPorNodo (Grafo &G, int mejor_color, int color_anterior, int mejor_nodo, vector<int>& colores) {
    //Miramos los vecinos del nodo modificado (mejor_nodo)
    if (mejor_color != color_anterior) {
        for (int i = 0; i < G[mejor_nodo].size(); ++i) {
            //Para cada vecino, indicamos que su cantidad de vecinos con mejor_color aumentó en uno
            colores_por_nodo[G[mejor_nodo][i]][mejor_color]++;
            //Para cada vecino del nodo modificado, reducimos en uno su cantidad de vecinos pintados con color_anterior
            colores_por_nodo[G[mejor_nodo][i]][color_anterior]--;
        }
    }
}

//Determina si los colores de dos nodos adyacentes en G son swapeables o no
bool swapeables (int i, int j, vector<int>& colores){
    return (colores_por_nodo[i][colores[j]]==1 && colores_por_nodo[j][colores[i]]==1);
}

//Determina en forma eficiente cómo cambia el impacto al cambiar el color de un nodo
int impactoResultanteChange (int nodo, int color_anterior, int color_nuevo, vector<vector<int>>& H,vector<int> & colores){
	int newImpact = impactoIteracionActual;
	//Recorremos los vecinos del nodo que cambia y nos fijamos si esa arista deja de colaborar o pasa a colaborar en el impacto
	for (int i = 0; i < H[nodo].size(); ++i){
		if (colores[H[nodo][i]]== color_anterior){
			newImpact--;
		}
		if (colores[H[nodo][i]]== color_nuevo){
			newImpact++;
		}
	}

	return newImpact;
}


//----------------Funciones de vecindad--------------------------
//Para hacer swaps, como no lo hacemos en todo momento, no consideramos que fuera necesaria una memoria de estructura
void vecindadSwap (Grafo &G, Grafo &H_Lista, int n,vector<int> &colores){
    int mejor_impacto_vecindad = 0;
    int mejor_nodo1 = 0;
    int mejor_nodo2 = 0;
    //Variables auxiliares para swapeo de colores
    int color_anterior1;
    int color_anterior2;
    bool vecindadNoVacia = false;
    for (int i = 0; i < n; ++i) {
        int j = 0;
        bool swapped = false;
        //Mirar los vecinos de i en G, y ver si es swapeable con alguno de ellos
        while(j<G[i].size() && !swapped) {
            if (swapeables(i,G[i][j],colores)){
                //Swapeamos los dos nodos y medimos el impacto
                color_anterior1 = colores[i];
                color_anterior2 = colores[G[i][j]];
                colores[i]=color_anterior2;
                colores[G[i][j]]=color_anterior1;
                swapped = true;
                vecindadNoVacia = true;
            }else{
                j++;
            }
        }
        //Si efectivamente encontró un nodo con que swappear, calculamos el impacto de swappearlos
        if (swapped) {
            int impacto_posible = impacto_lista(H_Lista,colores);
            if (impacto_posible > mejor_impacto_vecindad) {
                mejor_impacto_vecindad = impacto_posible;
                mejor_nodo1 = i;
                mejor_nodo2 = G[i][j];
            }
            //Volvemos a colocar los colores como estaban para poder seguir explorando la vecindad
            colores[i]= color_anterior1;
            colores[G[i][j]]= color_anterior2;
        }
    }

    //Swapeamos los dos nodos que nos dan mayor impacto
    if (vecindadNoVacia){
        color_anterior1 = colores[mejor_nodo1];
        color_anterior2 = colores[mejor_nodo2];
        colores[mejor_nodo1] = color_anterior2;
        colores[mejor_nodo2] = color_anterior1;
        //Actualizar colores_por_nodo
        for (int i = 0; i < G[mejor_nodo1].size(); ++i) {
            colores_por_nodo[G[mejor_nodo1][i]][color_anterior2]++;
            colores_por_nodo[G[mejor_nodo1][i]][color_anterior1]--;
        }
        for (int i = 0; i < G[mejor_nodo2].size(); ++i) {
            colores_por_nodo[G[mejor_nodo2][i]][color_anterior2]--;
            colores_por_nodo[G[mejor_nodo2][i]][color_anterior1]++;
        }
        if (mejor_impacto_vecindad > mejorImpactoActual) {
            mejorImpactoActual = mejor_impacto_vecindad;
            mejorSolucionActual = colores;
        }
        impactoIteracionActual = mejor_impacto_vecindad;
    }
}
void vecindadChangeColor (vector<int>& colores, int n, Grafo &G, Grafo &H_Lista, int& menorColorNoUsado){
    int mejor_impacto_vecindad = 0;
    int mejor_nodo = 0;
    int mejor_color = colores[0];
    int color_anterior;
    int randColor;
    bool vecindadNoVacia = false;
    //Para cada nodo que no esté en la memoria, elegimos con algún criterio un color al que cambiarlo
    //Entre todos los cambios posibles, se elige el de mayor impacto
    for (int j = 0; j < n; ++j) {

        if (!nodos_prohibidos[j]) {
            int impacto_posible=0;
            bool eligioColor = false;
            //Primer criterio: Se busca pintar el nodo con un color de algún vecino de H
            for (int l = 0; l < H_Lista[j].size(); ++l) {
                int colorVecinoEnH =colores[H_Lista[j][l]];
                if (colores_por_nodo[j][colorVecinoEnH]==0 && colorVecinoEnH!= colores[j]){
                    vecindadNoVacia = true;
                    color_anterior = colores [j];
                    colores[j] = colorVecinoEnH;
                    eligioColor = true;
                    break;
                }
            }
            //Segundo criterio: Si no se puede pintar con el color de ningún vecino en H, se elige el menor color no usado menor que el maximo color permitido
            //En caso de no haber, se elige al azar un número de color, y se elige el menor color mayor que ese, que resulta factible
            if (!eligioColor){
                if (menorColorNoUsado < colores_por_nodo[j].size()){
                    vecindadNoVacia = true;
                    color_anterior = colores[j];
                    colores[j] = menorColorNoUsado;
                    eligioColor = true;
                }else{
                    
                    randColor = rand()% colores_por_nodo[j].size();
                    while (randColor<colores_por_nodo[j].size() && colores_por_nodo[j][randColor]!=0){
                        randColor++;
                    }
                    if (randColor<colores_por_nodo[j].size()){
                        vecindadNoVacia =true;
                        color_anterior = colores[j];
                        colores[j] = randColor;
                        eligioColor = true;
                    }
                }
            }
            if (eligioColor){
                impacto_posible = impactoResultanteChange (j, color_anterior, colores[j],H_Lista,colores);
                if (impacto_posible > mejor_impacto_vecindad) {
                    mejor_impacto_vecindad = impacto_posible;
                    mejor_color = colores[j];
                    mejor_nodo = j;
                }
                colores[j] = color_anterior;
            }


        }
    }
    if(vecindadNoVacia) {
        color_anterior = colores[mejor_nodo];
        colores[mejor_nodo] = mejor_color;
        menorColorNoUsado = maximoColorUsado(colores) + 1;
        actualizarColoresPorNodo(G, mejor_color, color_anterior, mejor_nodo, colores);
        memoria.push(mejor_nodo);
        nodos_prohibidos[mejor_nodo] = true;
        int nodo_a_quitar = memoria.front();
        if (nodo_a_quitar != -1) {
            nodos_prohibidos[nodo_a_quitar] = false;
        }
        memoria.pop();
        impactoIteracionActual = mejor_impacto_vecindad;

        if (mejor_impacto_vecindad > mejorImpactoActual) {
            mejorImpactoActual = mejor_impacto_vecindad;
            mejorSolucionActual = colores;
        }
    }

}

void vecindadChangeColorVariable (vector<int>& colores, int n, Grafo &G, Grafo &H_Lista, int& menorColorNoUsado, int cantidadColoresVecindad){
	int mejor_impacto_vecindad = 0;
    int mejor_nodo = 0;
    int mejor_color = colores[0];
    int color_anterior;
    int randColor;
    bool vecindadNoVacia = false;
    //Para cada nodo que no esté en la memoria, elegimos con algún criterio un color al que cambiarlo
    //Entre todos los cambios posibles, se elige el de mayor impacto
    for (int j = 0; j < n; ++j) {
        if (!nodos_prohibidos[j]) {
            int impacto_posible=0;
            int k = 0;
            int cantidadColoresProbados = 0;
            while (k<colores_por_nodo[j].size() && cantidadColoresProbados<cantidadColoresVecindad){
            	if (colores_por_nodo[j][k]==0 && colores[j]!=k){
            		vecindadNoVacia = true;
            	    color_anterior = colores[j];
            		colores[j] = k;
            		impacto_posible = impacto_lista(H_Lista, colores);
                	if (impacto_posible > mejor_impacto_vecindad) {
                    	mejor_impacto_vecindad = impacto_posible;
                    	mejor_color = k;
                    	mejor_nodo = j;
                    }
                    colores[j] = color_anterior;
            		cantidadColoresProbados++;
                }
            	k++;
            }
        }
    }
    if (vecindadNoVacia) {
        color_anterior = colores[mejor_nodo];
        colores[mejor_nodo] = mejor_color;
        menorColorNoUsado = maximoColorUsado(colores) + 1;
        actualizarColoresPorNodo(G, mejor_color, color_anterior, mejor_nodo, colores);
        memoria.push(mejor_nodo);
        nodos_prohibidos[mejor_nodo] = true;
        int nodo_a_quitar = memoria.front();
        if (nodo_a_quitar != -1) {
            nodos_prohibidos[nodo_a_quitar] = false;
        }
        memoria.pop();

        if (mejor_impacto_vecindad > mejorImpactoActual) {
            mejorImpactoActual = mejor_impacto_vecindad;
            mejorSolucionActual = colores;
        }
        impactoIteracionActual = mejor_impacto_vecindad;
    }
}


//--------------------Metaheurística 2----------------------------
/* Parámetros de experimentación:
bool heuristica.  true: heurística1
                  false: heurística2
int max_it. Número máximo de iteraciones:
            0: n iteraciones
            1: n^2 iteraciones

int max_it_sin_mejora:
            0: no se tiene en cuenta
            1: raiz(cantidad iteraciones) iteraciones sin mejora
            2: cantidad iteraciones/2 iteraciones sin mejora

int max_cant_colores: máximo número de colores que se emplearán en los coloreos
            0: cantidad de colores usada por la heurística golosa (no se permite agregar colores)
            1: maxColorHeuristica +sqrt(n-maxColorHeuristica)
            2: n

int tipo_vecindad:
            0: vecindad por change color exclusivamente
            1: vecindad por change color y cada 5 iteraciones usar swap
            2: vecindad por change color y cada 10 iteraciones usar swap

int memory_size:
            se almacenara el `p`% de los nodos

int vecindad_size:
			0: vecindad probando solo un color por nodo, eligiendo "inteligentemente" los colores
			k: vecindad probando con k colores por nodo, elegidos sin criterio goloso
			-1: vecindad probando con raiz(n) colores por nodo
			-2: vecindad probando con n/2 colores por nodo

*/
void metaheuristica2(bool heuristica, int max_it, int max_it_sin_mejora, int max_cant_colores, int tipo_vecindad, int memory_size, int vecindad_size,
        vector<int> &colores, Grafo &G, Grafo &G_Matriz, Grafo &H_Lista, vector<tuple<int, int, int>> &aristasH, int mG, vector<int> &impactoVsIteraciones) {
    int n = G.size();
    srand (time(0));
    //Obtener solución inicial usando heurística golosa
    if (heuristica) {
        colores = vector<int> (n,UNDEFINED);
	    heuristica1(colores, G, G_Matriz, H_Lista);
	    mejorImpactoActual = impacto_lista(H_Lista, colores);
	    mejorSolucionActual = colores;
	    impactoVsIteraciones.push_back(mejorImpactoActual);
	    impactoIteracionActual = mejorImpactoActual;

    } else {
        colores = vector<int> (n,UNDEFINED);
        heuristica2(colores, G, G_Matriz, H_Lista, aristasH, mG,n);
        mejorImpactoActual = impacto_lista(H_Lista, colores);
        mejorSolucionActual = colores;
        impactoVsIteraciones.push_back(mejorImpactoActual);
        impactoIteracionActual = mejorImpactoActual;

    }

    //Inicialización de memoria de estructura
    nodos_prohibidos = vector<bool>(n, false);
    memory_size = memory_size * n /100;
    inicializarMemoria(memory_size);

    //Determinación de la cantidad de colores a usar, e inicialización de la estructura de información de colores vecinos
    int maxColor = maximoColorUsado(colores)+1;
    int menorColorNoUsado = maxColor + 1;
    if (max_cant_colores == 1) {
        maxColor = maxColor+floor(sqrt(n-maxColor));
    } else if (max_cant_colores==2){
        maxColor = n;
    }
    inicializarColoresPorNodo(colores, n, G, maxColor);

    max_it = pow(n,max_it+1);

    int iteraciones_sin_mejora = 0;

    if(max_it_sin_mejora==0){
        max_it_sin_mejora = INT_MAX;
    }else if (max_it_sin_mejora == 1){
        max_it_sin_mejora  = floor(sqrt(max_it));
    }else{
        max_it_sin_mejora = max_it/2;
    }


    if (vecindad_size==-1){
    	vecindad_size = floor(sqrt(n));
    } else if (vecindad_size==-2){
    	vecindad_size = n/2;
    }

    //Recorrer el espacio de soluciones
    
    for (int i = 0; i < max_it && iteraciones_sin_mejora<max_it_sin_mejora; ++i) {
        int impactoPrevio = mejorImpactoActual;
        if ( tipo_vecindad!=0 && i % (2*tipo_vecindad) == 0) {
            vecindadSwap(G, H_Lista, n, colores);
        } else {
            if (vecindad_size==0){
            	vecindadChangeColor(colores, n, G, H_Lista, menorColorNoUsado);
        	}else{
        		vecindadChangeColorVariable(colores,n,G,H_Lista,menorColorNoUsado,vecindad_size);
        	}
        }

        if(impactoPrevio==mejorImpactoActual){
            iteraciones_sin_mejora++;
        }else{
            iteraciones_sin_mejora = 0;
        }
        impactoVsIteraciones.push_back(impacto_lista(H_Lista,colores));
    }
colores = mejorSolucionActual;
}
