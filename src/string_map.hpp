#include "string_map.h"

template<typename T>
string_map<T>::string_map() : raiz(nullptr), _size(0), _claves() {
}

template<typename T>
string_map<T>::string_map(const string_map<T> &aCopiar) : string_map() {
    _size = aCopiar.size();
    *this = aCopiar;
}

template<typename T>
string_map<T> &string_map<T>::operator=(const string_map<T> &d) {
    recDelete(raiz);
    if (d.raiz != nullptr) {
        raiz = copiarSubtrie(d.raiz);
        _size = d._size;
        _claves = d._claves;
    }
    return *this;
}

template<typename T>
string_map<T>::~string_map() {
    recDelete(raiz);
}

template<typename T>
void string_map<T>::recDelete(Nodo *actual) {
    if (actual != nullptr) {
        for (int i = 0; i < actual->siguientes.size(); i++) {
            if (actual->siguientes[i] != nullptr){
            recDelete(actual->siguientes[i]);
            }
        }
        if (actual->definicion != nullptr) {
            delete actual->definicion;
        }
        delete actual;
    }
}

template<typename T>
void string_map<T>::insert(const pair<string, T> &entrada) {
    if (raiz == nullptr) {
        raiz = new Nodo;
    }
    Nodo *actual = raiz;
    int i = 0;
    while (i < entrada.first.size() and
           actual->siguientes[entrada.first[i]-32] != nullptr) { //Recorre los nodos ya existentes
        Nodo *sig = actual->siguientes[entrada.first[i]-32];
        actual = sig;
        i++;
    }
    while (i < entrada.first.size()) { //Define los que quedan
        Nodo *nuevo = new Nodo;
        actual->siguientes[entrada.first[i]-32] = nuevo;
        actual = nuevo;
        i++;
    }
    if (actual->definicion != nullptr) { //Agrega el significado
        *(actual->definicion) = entrada.second;
    } else {
        actual->definicion = new T(entrada.second);
        _claves.insert(entrada.first);
    }
    _size++;
}

template<typename T>
T &string_map<T>::operator[](const string &clave) {
    if (raiz == nullptr) {
        raiz = new Nodo;
    }
    Nodo *actual = raiz;
    int i = 0;
    while (i < clave.size() and
           actual->siguientes[clave[i]-32] != nullptr) { //Recorre los nodos ya existentes
        Nodo *sig = actual->siguientes[clave[i]-32];
        actual = sig;
        i++;
    }
    if (i == clave.size() and actual->definicion != nullptr) {
        return *actual->definicion;
    } else {
        while (i < clave.size()) { //Define los que quedan (si quedan)
            Nodo *nuevo = new Nodo;
            actual->siguientes[clave[i]-32] = nuevo;
            actual = nuevo;
            i++;
        }
        actual->definicion = new T();
        _size++;
        _claves.insert(clave);
        return *actual->definicion;
    }
}

template<typename T>
int string_map<T>::count(const string &clave) const {
    if (raiz == nullptr) {
        return 0;
    } else {
        Nodo *actual = raiz;
        int i = 0;
        while (i < clave.size()) {
            Nodo *sig = actual->siguientes[clave[i]-32];
            if (sig == nullptr) {
                return 0;
            }
            actual = sig;
            i++;
        }
        if (actual->definicion != nullptr) {
            return 1;
        } else {
            return 0;
        }
    }
}

template<typename T>
const T &string_map<T>::at(const string &clave) const {
    Nodo *actual = raiz;
    int i = 0;
    while (i < clave.size()) {
        actual = actual->siguientes[clave[i]-32];
        i++;
    }
    return *actual->definicion;
}

template<typename T>
T &string_map<T>::at(const string &clave) {
    Nodo *actual = raiz;
    int i = 0;
    while (i < clave.size()) {
        actual = actual->siguientes[clave[i]-32];
        i++;
    }
    return *actual->definicion;
}

template<typename T>
int cantHijos(vector<T *> hijos) {
    int res = 0;
    for (int i = 0; i < hijos.size(); i++) {
        if (hijos[i] != nullptr) {
            res++;
        }
    }
    return res;
}

template<typename T>
void string_map<T>::erase(const string &clave) {
    Nodo *actual = raiz;
    Nodo *ultimo = raiz;
    int ultPos = 0;
    for (int i = 0; i < clave.size(); i++) {
        if ((cantHijos(actual->siguientes) > 1 or actual->definicion != nullptr) and ultPos < clave.size() - 1) {
            ultimo = actual;
            ultPos = i;
        }
        actual = actual->siguientes[clave[i]-32];
    }
    delete actual->definicion;
    _size--;
    _claves.erase(clave);
    actual->definicion = nullptr;
    if (cantHijos(actual->siguientes) == 0) {
        Nodo *siguiente = ultimo->siguientes[clave[ultPos]-32];
        ultimo->siguientes[clave[ultPos]-32] = nullptr;
        ultimo = siguiente;
        ultPos++;
        while (ultPos < clave.size()) {
            siguiente = ultimo->siguientes[clave[ultPos]-32];
            delete ultimo;
            ultimo = siguiente;
            ultPos++;
        }
        delete ultimo;
    }
}

template<typename T>
int string_map<T>::size() const {
    return _size;
}

template<typename T>
const set<string>& string_map<T>::claves() const{
    return _claves;
}

template<typename T>
bool string_map<T>::empty() const {
    return raiz == nullptr;
}
