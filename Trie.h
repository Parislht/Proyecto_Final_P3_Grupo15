#ifndef PROYECTO_FINAL_TRIE_H
#define PROYECTO_FINAL_TRIE_H
#include <unordered_map>
#include <vector>
#include <string>
#include <memory>
using namespace std;

template<typename T>
struct Trie_nodo {
    unordered_map<char, unique_ptr<Trie_nodo<T>>> hijos;
    vector<T> datos;
    Trie_nodo() = default;
};

template<typename T>
class arbol_trie {
private:
    unique_ptr<Trie_nodo<T>> raiz;

public:
    arbol_trie();
    void insertar(const string& clave, const T& dato);
    vector<T> buscar_general(const string& consulta, size_t start_index = 0, size_t max_results = 5) const;

private:
    const Trie_nodo<T>* buscar_nodo(const string& prefijo) const;
    void buscar_aux(const Trie_nodo<T>* nodo, string prefijo, vector<T>& resultados) const;
    void buscar_subcadena_aux(const Trie_nodo<T>* nodo, const string& subcadena, const string& titulo_actual, vector<T>& resultados) const;
};

#include "Trie.tpp"

#endif //PROYECTO_FINAL_TRIE_H
