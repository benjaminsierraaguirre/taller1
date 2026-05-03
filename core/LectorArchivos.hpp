#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include "../data_structures/ListaEnlazada.hpp"
#include "../classes/Cancion.hpp"

using namespace std;

class LectorArchivos {
public:
    // carga canciones desde music_source.txt 
    static void cargarDesdeTexto(ListaEnlazada<Cancion>& lista, string nombreArchivo);

    // guarda todas las canciones al archivo (reescribe completo)
    static void guardarCatalogo(ListaEnlazada<Cancion>& lista, string nombreArchivo);
};