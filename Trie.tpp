#include "Trie.h"

template<typename T>
arbol_trie<T>::arbol_trie() : raiz(make_unique<Trie_nodo<T>>()) {}

template<typename T>
void arbol_trie<T>::insertar(const string& clave, const T& dato) {
    Trie_nodo<T>* nodo = raiz.get();
    for (char c : clave) {
        if (nodo->hijos.find(c) == nodo->hijos.end()) {
            nodo->hijos[c] = make_unique<Trie_nodo<T>>();
        }
        nodo = nodo->hijos[c].get();
    }
    nodo->datos.push_back(dato);
}

template<typename T>
vector<T> arbol_trie<T>::buscar_general(const string& consulta, size_t start_index, size_t max_results) const {
    vector<T> resultados;
    if (consulta.empty()) {
        buscar_aux(raiz.get(), "", resultados);
    } else {
        const Trie_nodo<T>* nodo = buscar_nodo(consulta);
        if (nodo) {
            buscar_aux(nodo, consulta, resultados);
        } else {
            buscar_subcadena_aux(raiz.get(), consulta, "", resultados);
        }
    }

    vector<T> resultado_paginado;
    size_t end_index = min(start_index + max_results, resultados.size());
    for (size_t i = start_index; i < end_index; ++i) {
        resultado_paginado.push_back(resultados[i]);
    }

    return resultado_paginado;
}

template<typename T>
const Trie_nodo<T>* arbol_trie<T>::buscar_nodo(const string& prefijo) const {
    const Trie_nodo<T>* nodo = raiz.get();
    for (char c : prefijo) {
        if (nodo->hijos.find(c) == nodo->hijos.end()) {
            return nullptr;
        }
        nodo = nodo->hijos.at(c).get();
    }
    return nodo;
}

template<typename T>
void arbol_trie<T>::buscar_aux(const Trie_nodo<T>* nodo, string prefijo, vector<T>& resultados) const {
    if (!nodo->datos.empty()) {
        for (const auto& pelicula : nodo->datos) {
            resultados.push_back(pelicula);
        }
    }

    for (const auto& pair : nodo->hijos) {
        buscar_aux(pair.second.get(), prefijo + pair.first, resultados);
    }
}

template<typename T>
void arbol_trie<T>::buscar_subcadena_aux(const Trie_nodo<T>* nodo, const string& subcadena, const string& titulo_actual, vector<T>& resultados) const {
    if (!nodo->datos.empty()) {
        for (const auto& pelicula : nodo->datos) {
            if (titulo_actual.find(subcadena) != string::npos) {
                resultados.push_back(pelicula);
            }
        }
    }

    for (const auto& pair : nodo->hijos) {
        buscar_subcadena_aux(pair.second.get(), subcadena, titulo_actual + pair.first, resultados);
    }
}