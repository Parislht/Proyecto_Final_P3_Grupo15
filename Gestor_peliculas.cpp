#include "Gestor_peliculas.h"

Gestor_peliculas_singleton* Gestor_peliculas_singleton::gestor = nullptr;

Gestor_peliculas_singleton* Gestor_peliculas_singleton::get_instance() {
    if (gestor == nullptr) {
        gestor = new Gestor_peliculas_singleton();
    }
    return gestor;
}

void Gestor_peliculas_singleton::cargar_peliculas() {
    ifstream file("C:\\Users\\paris\\CLionProjects\\Proyecto_final\\peliculas_modificado_final.csv");
    if (!file.is_open()) {
        cerr << " No se pudo abrir el archivo. " << endl;
        return;
    }

    file.seekg(0, ios::end);
    size_t file_size = file.tellg();
    size_t mitad = file_size / 2;

    file.seekg(mitad);
    while (file.peek() != '\n' && file.tellg() < file_size) {
        file.seekg(1, ios::cur);
    }

    size_t pos_hilo1 = file.tellg();
    file.close();

    cout << endl << " Archivo abierto correctamente \n" << endl;

    thread t1(&Gestor_peliculas_singleton::cargar_peliculas_rango, this,
              "C:\\Users\\paris\\CLionProjects\\Proyecto_final\\peliculas_modificado_final.csv",
              0, pos_hilo1, ref(contador_hilo1));

    thread t2(&Gestor_peliculas_singleton::cargar_peliculas_rango, this,
              "C:\\Users\\paris\\CLionProjects\\Proyecto_final\\peliculas_modificado_final.csv",
              pos_hilo1, file_size, ref(contador_hilo2));

    t1.join();
    t2.join();

    cout << "El primer hilo proceso " << contador_hilo1 << " lineas." << endl;
    cout << "El segundo hilo proceso " << contador_hilo2 << " lineas. " << endl;
    cout << "Lineas procesadas en total : " << contador_hilo2 + contador_hilo1 << " lineas. \n" << endl;

    cargar_ver_mas_tarde();

}

void Gestor_peliculas_singleton::cargar_peliculas_rango(const string& filename, size_t start, size_t end, int& contador) {
    ifstream file(filename);
    file.seekg(start);
    string line;
    if (start != 0) {
        getline(file, line);
    }
    while (file.tellg() < end && getline(file, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string index, imdb_id, title, plot_synopsis, tags_str, split, synopsis_source;

        getline(ss, index, '\"');
        getline(ss, index, '\"');
        ss.get();
        getline(ss, imdb_id, '\"');
        getline(ss, imdb_id, '\"');
        ss.get();
        getline(ss, title, '\"');
        getline(ss, title, '\"');
        ss.get();
        getline(ss, plot_synopsis, '\"');
        getline(ss, plot_synopsis, '\"');
        ss.get();
        getline(ss, tags_str, '\"');
        getline(ss, tags_str, '\"');
        ss.get();
        getline(ss, split, '\"');
        getline(ss, split, '\"');
        ss.get();
        getline(ss, synopsis_source, '\"');
        getline(ss, synopsis_source, '\"');

        vector<string> tags;
        stringstream tags_ss(tags_str);
        string tag;
        while (getline(tags_ss, tag, ',')) {
            tags.push_back(tag);
        }

        Pelicula pelicula(imdb_id, title, plot_synopsis, tags, split, synopsis_source);

        lock_guard<mutex> lock(mtx);
        arbol_busqueda.insertar(title, pelicula);
        for (const auto& t : tags) {
            arbol_busqueda.insertar(t, pelicula);
        }

        contador++;
    }
}

void Gestor_peliculas_singleton::buscar_general(const string& consulta, size_t start_index, size_t max_results) {

    vector<Pelicula> resultados = arbol_busqueda.buscar_general(consulta, start_index, max_results);

    for (const auto& pelicula : resultados) {
        pelicula.mostrar_informacion();

        char respuesta;
        cout << "Agregar a 'ver mas tarde'? (s/n): "; cin >> respuesta;
        if (respuesta == 's') {
            agregar_a_ver_mas_tarde(pelicula.get_titulo());
        }
    }

    if (resultados.size() == max_results) {
        char respuesta;
        cout << "Mostrar mas resultados? (s/n): "; cin >> respuesta;
        if (respuesta == 's') {
            buscar_general(consulta, start_index + max_results, max_results);
        }
    }
}

void Gestor_peliculas_singleton::buscar_tag(const string & s, size_t start_index, size_t max_results) {

    vector<Pelicula> resultados = arbol_busqueda.buscar_general(s, start_index, max_results);

    for (const auto& pelicula : resultados) {
        pelicula.mostrar_informacion();

        cout << "Agregar a 'ver mas tarde'? (s/n): ";
        char respuesta;
        cin >> respuesta;
        if (respuesta == 's') {
            agregar_a_ver_mas_tarde(pelicula.get_titulo());
        }
    }

    if (resultados.size() == max_results) {
        cout << "Mostrar mas resultados? (s/n): ";
        char respuesta;
        cin >> respuesta;
        if (respuesta == 's') {
            buscar_tag(s, start_index + max_results, max_results);
        }
    }
}

void Gestor_peliculas_singleton::cargar_ver_mas_tarde() {
    ifstream file("C:\\Users\\paris\\CLionProjects\\Proyecto_final\\ver_mas_tarde.txt");
    if (!file.is_open()) {
        cerr << endl << endl <<"No se pudo abrir el archivo de 'ver mas tarde'." << endl;
        return;
    }

    string imdb_id;
    while (getline(file, imdb_id)) {
        if (!imdb_id.empty()) {
            ver_mas_tarde.push_back(imdb_id);
        }
    }
    file.close();
}

void Gestor_peliculas_singleton::mostrar_ver_mas_tarde(size_t start_index, size_t max_results) {
    cout << "Mostrando peliculas guardadas en 'ver mas tarde':" << endl;

    if (ver_mas_tarde.empty()) {
        cout << "No hay peliculas guardadas en 'ver mas tarde'." << endl;
        return;
    }

    size_t end_index = min(start_index + max_results, ver_mas_tarde.size());
    for (size_t i = start_index; i < end_index; ++i) {
        cout << "Buscando pelicula con Titulo: " << ver_mas_tarde[i] << endl;
        auto resultados = arbol_busqueda.buscar_general(ver_mas_tarde[i]);
        if (!resultados.empty()) {
            resultados[0].mostrar_informacion();
        } else {
            cout << "No se encontro la pelicula con titulo: " << ver_mas_tarde[i] << endl;
        }
    }

    if (end_index < ver_mas_tarde.size()) {
        cout << "Mostrar mas resultados? (s/n): ";
        char respuesta;
        cin >> respuesta;
        if (respuesta == 's') {
            mostrar_ver_mas_tarde(end_index, max_results);
        }
    }
}

void Gestor_peliculas_singleton::agregar_a_ver_mas_tarde(const string& titulo) {
    ofstream file("C:\\Users\\paris\\CLionProjects\\Proyecto_final\\ver_mas_tarde.txt", ios::app);
    if (file.is_open()) {
        file << titulo << endl;
        file.close();
    } else {
        cerr << "No se pudo abrir el archivo para guardar 'ver más tarde'." << endl;
    }
}






Cargador_peliculas_proxy_singleton* Cargador_peliculas_proxy_singleton::cargador = nullptr;

Cargador_peliculas_proxy_singleton::Cargador_peliculas_proxy_singleton ()
        : carga(false),
          gestor(Gestor_peliculas_singleton::get_instance ()) {}

Cargador_peliculas_proxy_singleton* Cargador_peliculas_proxy_singleton::get_instance_cargador_proxy() {
    if (cargador == nullptr) {
        cargador = new Cargador_peliculas_proxy_singleton();
    }
    return cargador;
}

void Cargador_peliculas_proxy_singleton::cargar_peliculas () {
    if (!carga) {
        gestor->cargar_peliculas();
        carga = true;
    } else {
        cout << "Las peliculas ya han sido cargadas previamente." << endl;
    }
}

void Cargador_peliculas_proxy_singleton::buscar_general (const string& consulta, size_t start_index, size_t max_results) {
    gestor->buscar_general(consulta, start_index, max_results);
}

void Cargador_peliculas_proxy_singleton::buscar_tag (const string & s, size_t start, size_t max) {
    gestor->buscar_tag(s, start, max );
}
