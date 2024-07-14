#ifndef PROYECTO_FINAL_PELICULA_H
#define PROYECTO_FINAL_PELICULA_H
#include <iostream>
#include <vector>
#include <string>
using namespace std;

class Pelicula {
private:
    string indice, titulo, plot_synopsis, split, synopsis_source, imdb_id;
    vector<string> tags;

public:
    Pelicula(const string& imdb_, const string& t, const string syn, const vector<string>& ta, const string& spl, const string& syn_source);
    string get_titulo() const;
    string get_plot_synopsis() const;
    vector<string> get_tags() const;
    string get_imdb_id() const;
    void mostrar_informacion() const;
};

#endif //PROYECTO_FINAL_PELICULA_H
