#pragma once
#include <iostream>
#include <string>
#include "../data_structures/ListaEnlazada.hpp"
#include "../data_structures/Stack.hpp"
#include "../classes/Cancion.hpp"

using namespace std;

class Menu {
public:
    // limpiar la consola 
    static void limpiarPantalla();

    // mostrar la pantalla principal
    static void mostrarPrincipal(Cancion actual, string estado,
                                  bool aleatorio, string repeticion,
                                  bool hayHistorial);

    // opcion A: mostrar la lista de reproducccion
    static void mostrarCola(Cancion actual, ListaEnlazada<Cancion>& cola,
                             bool aleatorio, string repeticion);

    // opción L: ver el catálogo completo de canciones en el archivo
    static void mostrarCatalogo(Cancion actual, ListaEnlazada<Cancion>& catalogo,
                                 bool aleatorio, string repeticion);

    // solicitar datos para agregar una nueva canción
    static Cancion pedirDatosCancion(int nuevoId);
};