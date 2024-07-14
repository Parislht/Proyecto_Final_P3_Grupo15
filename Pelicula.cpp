#include "Pelicula.h"
#include <algorithm>

Pelicula::Pelicula(const string& imdb_, const string& t, const string syn, const vector<string>& ta, const string& spl, const string& syn_source)
        : imdb_id(imdb_), titulo(t), plot_synopsis(syn), tags(ta), split(spl), synopsis_source(syn_source) {}

string Pelicula::get_titulo() const { return titulo; }
string Pelicula::get_plot_synopsis() const { return plot_synopsis; }
vector<string> Pelicula::get_tags() const { return tags; }
string Pelicula::get_imdb_id() const { return imdb_id; }

void Pelicula::mostrar_informacion() const {

    cout << "---------------------------------------------------------------------------------" << endl;
    cout << "IMDB_ID  :  " << imdb_id << endl;
    cout << "TITULO   :  " << titulo << endl;
    cout << "SINOPSIS :  " << endl<< plot_synopsis << endl;
    cout << "" << endl;
    cout << "TAGS     : ";
    for_each(tags.begin(), tags.end(), [](const string& s){ cout << s << " , "; });
    cout << endl << "SPLIT    :  " << split << endl;
    cout << "SYNOPSIS_SOURCE  :  " << synopsis_source << endl;
    cout << "---------------------------------------------------------------------------------" << endl;
    cout << endl;
}
