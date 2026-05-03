#include "Cancion.hpp"

//constructor vacio
Cancion::Cancion() {
    id = 0;
    nombre = "";
    artista = "";
    album = "";
    anio = 0;
    duracion = 0;
    ruta = "";
}
//constructor con valores
Cancion::Cancion(int _id, string _n, string _art, string _alb, int _a, int _d, string _r) {
    id = _id;
    nombre = _n;
    artista = _art;
    album = _alb;
    anio = _a;
    duracion = _d;
    ruta = _r;
}