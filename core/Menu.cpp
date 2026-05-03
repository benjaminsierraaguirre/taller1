#include "Menu.hpp"


//modo actual (s-r1) o (ra), o (s), siendo s = aleatorio, y r= repeticion
//static porque solo lo uso en menu, no la declaro en hpp porque las demas clases no la utilizan
static string construirModo(bool aleatorio, string repeticion) { 
    string modo = "";

    if (aleatorio) {
        modo += "S";
    }
    if (repeticion != "") {
        if (modo != "") modo += "-";
        modo += repeticion;
        
    }
    if (modo != "") {
        modo = "(" + modo + ")";
    }

    return modo;
}

// limpio la consola (como usar clear en python)
void Menu::limpiarPantalla() {
    #ifdef _WIN32
        system("cls");  // Windows
    #else
        system("clear"); // Linux / mac
    #endif
}


// pantalla principal del reproductor
void Menu::mostrarPrincipal(Cancion actual, string estado, bool aleatorio, string repeticion, bool hayHistorial) {
    limpiarPantalla();
    string modo = construirModo(aleatorio, repeticion); //modo repeticion, modo aleatorio, ambas

    cout << "========================================" << endl;
    if (actual.id == 0) {
        cout << "  Reproduccion Detenida" << endl;
    } else {
        cout << "  " << estado;
        if (modo != "") cout << " " << modo;
        cout << ": " << actual.nombre << endl;
        cout << "  Artista: " << actual.artista << endl;
        cout << "  Album: " << actual.album << " [" << actual.anio << "]" << endl;
    }
    cout << "========================================" << endl;
    cout << "Opciones:" << endl;
    cout << "  W - Reproducir/Pausar" << endl;
    if (hayHistorial) {
        cout << "  Q - Pista Anterior" << endl;
    }
    cout << "  E - Pista Siguiente" << endl;

    // Mostrar estado actual de cada modo entre corchetes
    if (aleatorio) {
        cout << "  S - Activar/Desactivar modo aleatorio [ON]" << endl;
    } else {
        cout << "  S - Activar/Desactivar modo aleatorio [OFF]" << endl;
    }

    if (repeticion == "") {
        cout << "  R - Repeticion [Desactivado]" << endl;
    } else if (repeticion == "R1") {
        cout << "  R - Repeticion [Repetir una]" << endl;
    } else {
        cout << "  R - Repeticion [Repetir todas]" << endl;
    }

    cout << "  A - Ver lista de reproduccion actual" << endl;
    cout << "  L - Listado de canciones" << endl;
    cout << "  X - Salir" << endl;
    cout << "----------------------------------------" << endl;
    cout << "Ingrese Opcion: ";
}


// opcion A: cola de reproduccion actual
void Menu::mostrarCola(Cancion actual, ListaEnlazada<Cancion>& cola, bool aleatorio, string repeticion) {
    limpiarPantalla();
    string modo = construirModo(aleatorio, repeticion); //modo repeticion, modo aleatorio, ambas

    cout << "========================================" << endl;
    cout << "  Actual";
    if (modo != "") cout << " " << modo;
    cout << ": ";
    if (actual.id == 0) {
        cout << "Ninguna";
    } else {
        cout << actual.nombre << " - " << actual.artista;
    }
    cout << endl;
    cout << "========================================" << endl;
    cout << "Lista de reproduccion actual:" << endl;

    if (cola.getSize() == 0) {
        cout << "  Vacia" << endl;
        cout << "----------------------------------------" << endl;
        cout << "Opciones:" << endl;
        cout << "  V - Volver al menu principal" << endl;
    } else {
        Node<Cancion>* nodo = cola.getHead();
        int i = 1;
        while (nodo != nullptr) {
            cout << "  " << i << ". " << nodo->data.nombre << " - " << nodo->data.artista << endl; 
            nodo = nodo->next;
            i++;
        }
        cout << "----------------------------------------" << endl;
        cout << "Opciones:" << endl;
        cout << "  S<num> - Saltar a la cancion seleccionada (ejemplo: S2)" << endl;
        cout << "  V      - Volver al menu principal" << endl;
    }
    cout << "----------------------------------------" << endl;
    cout << "Ingrese Opcion: ";
}

// lista de canciones en archivo (letra L)
void Menu::mostrarCatalogo(Cancion actual, ListaEnlazada<Cancion>& catalogo, bool aleatorio, string repeticion) {
    limpiarPantalla();
    string modo = construirModo(aleatorio, repeticion); //modo repeticion, modo aleatorio, ambas

    cout << "========================================" << endl;
    cout << "  Actual";
    if (modo != "") cout << " " << modo;
    cout << ": ";
    if (actual.id == 0) {
        cout << "Ninguna";
    } else {
        cout << actual.nombre << " - " << actual.artista;
    }
    cout << endl;
    cout << "========================================" << endl;
    cout << "Canciones registradas:" << endl;

    Node<Cancion>* nodo = catalogo.getHead();
    int i = 1;
    while (nodo != nullptr) {
        cout << "  " << i << ". " << nodo->data.nombre << " - " << nodo->data.artista << endl;
        nodo = nodo->next;
        i++;
    }

    cout << "----------------------------------------" << endl;
    cout << "Opciones:" << endl;
    cout << "  R<num> - Reproducir cancion (ejemplo: R3)" << endl;
    cout << "  A<num> - Agregar al final de la cola (ejemplo: A5)" << endl;
    cout << "  N      - Agregar nueva cancion al registro" << endl;
    cout << "  D<num> - Eliminar cancion (ejemplo: D2)" << endl;
    cout << "  V      - Volver al menu principal" << endl;
    cout << "----------------------------------------" << endl;
    cout << "Ingrese Opcion: ";
}


//pedir datos de la nueva cancion
Cancion Menu::pedirDatosCancion(int nuevoId) {
    Cancion nueva;
    nueva.id = nuevoId;
    string anioStr, durStr;

    cout << "\n--- Nueva Cancion (ID: " << nuevoId << ") ---" << endl;

    cout << "Nombre de la cancion: ";
    cin.ignore(); //limpiamos el bufer
    getline(cin, nueva.nombre); 

    cout << "Artista: ";
    getline(cin, nueva.artista);

    cout << "Album: ";
    getline(cin, nueva.album);

    cout << "Año de lanzamiento: ";
    getline(cin, anioStr);
    nueva.anio = stoi(anioStr);

    cout << "Duracion en segundos: ";
    getline(cin, durStr);
    nueva.duracion = stoi(durStr);

    cout << "Ruta del archivo (ejemplo: music/cancion.mp3): ";
    getline(cin, nueva.ruta);

    return nueva;
}