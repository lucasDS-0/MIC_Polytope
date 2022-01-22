// Metaheuristica 1 
#include "metaheuristica1.h"
//vector_to_string
using namespace std;

void tabuSearch1(vector<int> &vColores, Grafo &G_Lista, Grafo &G_Matriz, Grafo &H_Lista, 
                 vector <tuple<int, int, int>> &aristasH, int mG, bool heuristica, int max_it, 
                 int max_it_sin_mejora, int max_cant_colores, int aspiracion, int num_factor) {

	//  Cantidad de nodos
	int n = G_Lista.size();
	
	// Solución inicial
	if (heuristica) {
      	heuristica1(vColores, G_Lista, G_Matriz, H_Lista);
	} else {		
		heuristica2(vColores, G_Lista, G_Matriz, H_Lista, aristasH, mG, n);
	}	
	
	// Mejor solución actual
	vector<int> S = vColores;
	
	// Iteraciones máximas
	int cIteraciones = (max_it == 0) ? n : (max_it == 1) ? n*n : n*n*n;	

	// Iteraciones máximas sin cambios
	int iteracionesSinC = 0;
	int maxSinMejora = (max_it_sin_mejora == 1) ? (int) sqrt(cIteraciones) : cIteraciones/2;
	
	// Cantidad máxima de colores a utilizar
	int colorMaximoAUtilizar = 0;			
	if (max_cant_colores == 0) {
		vector<int> aux = S;
		sort(aux.begin(), aux.end());
		int total = 1;
		for (int i = 1; i < aux.size(); i++) {	
			if (aux[i-1] != aux[i]) total++;
		}
		colorMaximoAUtilizar = total;
	} else if (max_cant_colores == 1) {
		auto itS = max_element(S.begin(), S.end());
		int color = S[distance(S.begin(), itS)];
		colorMaximoAUtilizar = color + (int) sqrt(n - color);
	} else {
		colorMaximoAUtilizar = n+1;
	}
	
	// Creación de la memoria
    string_map<int> memoria;
    
    // Función de aspiración
    int iteracionesPermitidasAsp = (aspiracion == 0) ? n : (aspiracion == 1) ? n/2 : (int) sqrt(n);
    
	// Factor para nodos a cambiar
	int factor = (num_factor == 0) ? n/2 : (int) sqrt(n);
	 	
	// Mejor por iteracion
	vector<int> S_aux = vColores;
	// Indice para la guarda
	int i = 0;
 	// Ciclo principal
	while (i < cIteraciones) {
	
		// Creación de la vecindad
		vector<vector<int>> vecindad;
		vector<int> vecina = S_aux;
					
		// El factor determina cuandos nodos modificaran su color en esta iteración
		vector<int> aCambiar = {}; 
		for (int j = 0; j < n; j++) aCambiar.push_back(j);
		for (int j = factor; j < n; j++) {
			int adicional = (int) rand() % aCambiar.size();
			aCambiar.erase(aCambiar.begin() + adicional);
		}
		
		for (int j = 0; j < aCambiar.size(); j++){
			int original = vecina[aCambiar[j]];
			for (int c = 0; c <= colorMaximoAUtilizar; c++){
				if (c != original) vecina[aCambiar[j]] = c;			
				if (valido(vecina, G_Lista)) vecindad.push_back(vecina);
			}
			vecina[aCambiar[j]] = original;
		}

		// Elección de la mejor solución de la vecindad
		int indice_s_aux = -1; // Por si toda la vecindad está en memoria		
		int impacto_aux = 0;

        for (int j = 0; j < vecindad.size(); j++) {

			string coloreo = vector_to_string(vecindad[j]);

			bool inMem = (bool) memoria.count(coloreo);
          
			bool permitida = inMem && ((i - memoria[coloreo]) > iteracionesPermitidasAsp);
			if (permitida || (!inMem && impacto_aux <= calculo_impacto_lista(H_Lista,vecindad[j]))){
				indice_s_aux = j;
                impacto_aux = calculo_impacto_lista(H_Lista,vecindad[j]);
			}
        }
        
	 	// Agregar a la memoria si es posible
		if (indice_s_aux != -1){
			S_aux = vecindad[indice_s_aux];
			memoria[vector_to_string(vecindad[indice_s_aux])] = i;
        }
	 	
		// Revisar si hay que actualizar la mejor solución
		if (indice_s_aux != -1 && impacto_aux >= calculo_impacto_lista(H_Lista,S)) {
	 		S = vecindad[indice_s_aux];
	 		iteracionesSinC = 0;
		} else {
			if (max_it_sin_mejora != 0) {
				iteracionesSinC++;
				i = (iteracionesSinC >= maxSinMejora) ? cIteraciones : i; 
			}
		}

		// Incremento de indice de la guarda
		i++;
				
	}
	
	vColores = S;
	
}
