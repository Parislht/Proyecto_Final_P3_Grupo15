#include "Gestor_peliculas.h"

int main() {
    Cargador_peliculas_proxy_singleton* cargador = Cargador_peliculas_proxy_singleton::get_instance_cargador_proxy();
    cargador->cargar_peliculas();

    string s;
    int opcion;
    cout << "--- Bienvenido a la plataforma de streaming--- \n"<<endl;
    cout << "por favor ingrese su tipo de busqueda \n"<<endl;
    cout << "Oprima 1 para busquedas generales "<<endl<<"oprima 2 para busquedas popr tags " << endl;
    cout << "Oprima 3 para ver peliculas guardadas en 'ver mas tarde' " << endl;
    cout << "Tipo de busqueda : ";cin>>opcion;

    if(opcion == 1){
        cout << endl<< "Ingrese consulta de busqueda: "; cin >> s;
        cargador->buscar_general(s);
    }
    else if(opcion == 2){
        cout << endl<< "Ingrese tag de busqueda: "; cin >> s;
        cargador->buscar_tag(s);
    }else if (opcion == 3) {
        Gestor_peliculas_singleton* gestor = Gestor_peliculas_singleton::get_instance();
        gestor->mostrar_ver_mas_tarde(0, 5);
    }

    return 0;
}
