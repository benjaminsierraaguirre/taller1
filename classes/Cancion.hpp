#pragma once
#include <iostream>
#include <string>

using namespace std;

class Cancion {
public:
    int id;
    string nombre;
    string artista;
    string album;
    int anio;
    int duracion;
    string ruta;

    Cancion();
    Cancion(int id, string n, string art, string alb, int a, int d, string r);
};