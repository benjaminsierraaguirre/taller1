#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>  // rand, srand
#include <ctime>    // time
#include "../data_structures/ListaEnlazada.hpp"
#include "../data_structures/Stack.hpp"
#include "../classes/Cancion.hpp"

using namespace std;

//asignaciomos estados de repeticion FIJOS (para las comparaciones rep = rep_ninguna)..
const int REP_NINGUNA = 0;  // sin repeticion
const int REP_UNA     = 1;  // repetir solo la cancion actual
const int REP_TODAS   = 2;  // repetir toda la lista cuando se acabe

class Reproductor {
public:
    ListaEnlazada<Cancion> catalogo;      // todas las canciones del ARCHIVO (catologo completo)
    ListaEnlazada<Cancion> cola;          // lista de reproduccion (pendientes por escuchar)
    Stack<Cancion>         historial;     // canciones ya escuchadas (para volver atrás)
    Cancion                actual;        // la cancion sonando ahora
    bool                   reproduciendo; // true = reproduciendo, false = en Pausa
    bool                   aleatorio;     // true = modo aleatorio activado
    int                    repeticion;    // usa las constantes que cree arriba (0, 1 o 2)

    Reproductor();

    // Navegacion
    void siguientePista();
    void pistaAnterior();

    // modos
    void activarDesactivarAleatorio();
    void cambiarRepeticion();

    // cola / lista reproduccion
    void agregarACola(Cancion c);
    void saltarACola(int posicion);  // posicion es 1-indexed (como el menu)
    void llenarCola();               // Llena la cola con el catalogo completo

    // Catalogo
    void agregarCancion(Cancion c);
    bool eliminarCancion(int idTarget);

    // lectura y escritura de la configuracion (status.cfg)
    void cargarEstado(string archivo);
    void guardarEstado(string archivo);

    // funciones de texto para mostrar en pantalla
    string getEstadoTexto();      // devuelve "Reproduciendo", "En Pausa" o "Reproduccion Detenida"
    string getRepeticionTexto();  // devuelve "", "R1" o "RA"
    bool   hayCanciones();
    int    proximoId();           // devuelve el siguiente ID disponible

private:
    void mezclarCola();    // algoritmo Fisher-Yates para mezclar la cola
    int  maxIdCatalogo();  // Busca el id mas alto en el catalogo
};