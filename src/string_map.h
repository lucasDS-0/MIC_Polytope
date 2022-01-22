#ifndef STRING_MAP_H_
#define STRING_MAP_H_

#include <string>
#include <vector>
#include <set>

using namespace std;

template<typename T>
class string_map {
public:
    // CONSTRUCTOR: Construye un diccionario vacio.
    string_map();

    // CONSTRUCTOR POR COPIA: Construye un diccionario por copia.
    string_map(const string_map<T> &aCopiar);

    // OPERADOR ASIGNACION
    string_map &operator=(const string_map &d);

    // DESTRUCTOR
    ~string_map();

    // INSERT: Inserta un par clave, valor en el diccionario
    void insert(const pair<string, T> &);

    // COUNT: Devuelve la cantidad de apariciones de la clave (0 o 1). Identifica si una clave está definida o no.
    int count(const string &clave) const;

    // AT: Dada una clave, devuelve su significado. PRE: La clave está definida. PRODUCE ALIASING
    // Versión no modificable
    const T &at(const string &clave) const;

    // Versión modificable
    T &at(const string &clave);

    // ERASE: Dada una clave, la borra del diccionario junto a su significado. PRE: La clave está definida. PRODUCE ALIASING
    void erase(const string &clave);

    // SIZE: Devuelve cantidad de claves definidas
    int size() const;

    // EMPTY: devuelve true si no hay ningún elemento en el diccionario
    bool empty() const;

    // CLAVES: Devuelve el conjunto de claves definidas en el diccionario
    const set<string>& claves() const;

    // operator[]: Acceso o definición de pares clave/valor
    T &operator[](const string &clave);

private:

    struct Nodo {
        vector<Nodo *> siguientes;
        T *definicion;

        Nodo() : definicion(nullptr), siguientes(vector<Nodo*> (137, nullptr)){
        };
    };

    Nodo *raiz;
    int _size;
    set<string> _claves;

    void recDelete(Nodo *actual);

    Nodo* copiarSubtrie(Nodo *actual){
        Nodo *nuevo = new Nodo;
        if (actual->definicion != nullptr) {
            nuevo->definicion = new T(*actual->definicion);
        }
        for (int i = 0; i < actual->siguientes.size(); i++) {
            if (actual->siguientes[i] != nullptr) {
                nuevo->siguientes[i] = copiarSubtrie(actual->siguientes[i]);
            }
        }
        return nuevo;
    };

};

#include "string_map.hpp"

#endif // STRING_MAP_H_
