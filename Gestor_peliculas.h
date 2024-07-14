#ifndef PROYECTO_FINAL_GESTOR_PELICULAS_H
#define PROYECTO_FINAL_GESTOR_PELICULAS_H
#include "Trie.h"
#include "Pelicula.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <thread>
#include <string>
#include <mutex>
#include <algorithm>

struct Gestor_peliculas_interfaz {
    virtual void cargar_peliculas() = 0;
    virtual void buscar_general(const string& consulta, size_t st, size_t max) = 0;
    virtual void buscar_tag    (const string& consulta,  size_t st, size_t max) = 0;
    virtual ~Gestor_peliculas_interfaz() = default;
};


class Gestor_peliculas_singleton : public Gestor_peliculas_interfaz {
private:
    static Gestor_peliculas_singleton* gestor;
    arbol_trie<Pelicula> arbol_busqueda;
    int contador_hilo1 = 0;
    int contador_hilo2 = 0;
    mutex mtx;
    vector<string> ver_mas_tarde;

    Gestor_peliculas_singleton() = default;

    void cargar_peliculas_rango(const string& filename, size_t start, size_t end, int& contador);
    void cargar_ver_mas_tarde();

public:
    static Gestor_peliculas_singleton* get_instance();
    void cargar_peliculas() override;
    void buscar_general (const string& consulta, size_t start_index = 0, size_t max_results = 5) override;
    void buscar_tag     (const string& consulta, size_t start_index = 0, size_t max_results = 5) override;
    void mostrar_ver_mas_tarde(size_t start_index = 0, size_t max_results = 5);
    void agregar_a_ver_mas_tarde(const string& imdb_id);

};


class Cargador_peliculas_proxy_singleton : public Gestor_peliculas_interfaz {
private:
    bool carga;
    static Cargador_peliculas_proxy_singleton* cargador;
    Gestor_peliculas_singleton* gestor;

    Cargador_peliculas_proxy_singleton();

public:
    static Cargador_peliculas_proxy_singleton* get_instance_cargador_proxy();
    void cargar_peliculas() override;
    void buscar_general (const string& consulta, size_t start_index = 0, size_t max_results = 5) override;
    void buscar_tag     (const string& consulta , size_t st = 0, size_t max = 5) override;
};

#endif //PROYECTO_FINAL_GESTOR_PELICULAS_H
