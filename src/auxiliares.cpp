#include "auxiliares.h"
#include <iostream>

int impacto_lista(Grafo &H, vector<int> &colores) {
    long unsigned int n = H.size();
    int impacto = 0;

    for (unsigned int i = 0; i < n; ++i) {
        int colorActual = colores[i];
        for (unsigned int j = 0; j < H[i].size(); ++j) {
            unsigned int v = H[i][j];
            if (v > i && colores[v] == colorActual) {
                impacto++;
            }
        }
    }

    return impacto;

}

int calculo_impacto_lista(Grafo &H, vector<int> &colores) {
    int impacto = 0;
    for (unsigned int i = 0; i < H.size(); i++) {
        for (unsigned int j = 0; j < H[i].size(); j++) {
            if (colores[i] == colores[H[i][j]]) impacto++;
        }
    }
    impacto /= 2;
    return impacto;
}

bool valido(vector<int> &colores, Grafo &G) {
    bool vale = true;
    for (unsigned int i = 0; i < G.size(); i++) {
        for (unsigned int j = 0; j < G[i].size(); j++) {
            if (colores[i] == colores[G[i][j]]) vale = false;
        }
    }
    return vale;
}

bool valido_undef(vector<int> &colores, Grafo &G) {
    bool vale = true;
    for (unsigned int i = 0; i < G.size(); i++) {
        for (unsigned int j = 0; j < G[i].size(); j++) {
            if (colores[i] == colores[G[i][j]] && colores[i] != UNDEFINED) vale = false;
        }
    }
    return vale;
}

void coloreo_es_valido(vector<int> &colores, Grafo &G) {
    for (unsigned int i = 0; i < G.size(); i++) {
        for (unsigned int j = 0; j < G[i].size(); j++) {
            if (colores[i] == colores[G[i][j]]) {
                cout << "ERROR EN COLOREO: " << endl;
                cout << "los nodos " << i << " y " << G[i][j] <<
                     " son adyacentes en G y están pintados de color " << colores[i] << endl;
                return;
            }
        }
    }
    cout << "Coloreo valido :)" << endl;
}

string vector_to_string(vector<int> &coloreo) {
    stringstream s;
    copy(coloreo.begin(), coloreo.end(), ostream_iterator<int>(s, ""));
    string copia = s.str();
    copia = copia.substr(0, copia.length() - 1);
    return copia;
}


void json_output(int n, int mG, int mH, int impacto, double total_time,
	             vector<int> &impactoVsIteraciones) {
	cout << "{";
	cout << "\"n\": " << n << ", ";
	cout << "\"mG\": " << mG << ", ";
	cout << "\"mH\": " << mH << ", ";
	cout << "\"impacto\": " << impacto << ", ";
	cout << "\"tiempo\": " << total_time;
	if (impactoVsIteraciones.size() > 0) {
		cout << ", \"impacto_vs_iteraciones\": [";
		for (unsigned i = 0; i < impactoVsIteraciones.size() - 1; i++) {
			cout << impactoVsIteraciones[i] << " ,";
		}
		cout << impactoVsIteraciones[impactoVsIteraciones.size() - 1] << "]";
	}
	cout << "}\n";
}
