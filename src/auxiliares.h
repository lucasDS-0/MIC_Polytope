#ifndef AUXILIARES_H
#define AUXILIARES_H

#include <iostream>
#include <sstream>
#include <iterator>

#include <vector>
#include <cstdio>
#include <tuple>
#include <list>
#include <queue>
#include <string>


#include <cstdio>
#include <cstdlib>
#include <stdlib.h>     /* srand, rand */

#include <cmath>
#include <time.h>       /* time */
#include <algorithm>
#include <bits/stdc++.h>

// color no seteado
#define UNDEFINED -1

using namespace std;

typedef vector <vector<int>> Grafo;

int impacto_lista(Grafo &H, vector<int> &colores);

int calculo_impacto_lista(Grafo &H, vector<int> &colores);

bool valido(vector<int> &colores, Grafo &G);

bool valido_undef(vector<int> &colores, Grafo &G);

void coloreo_es_valido(vector<int> &colores, Grafo &G);

string vector_to_string(vector<int> &coloreo);

void json_output(int n, int mG, int mH, int impacto, double total_time,
	             vector<int> &impactoVsIteraciones);

#endif
