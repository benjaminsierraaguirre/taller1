#include "LectorArchivos.hpp"


//carga las canciones desde music_source.txt a la lista
void LectorArchivos::cargarDesdeTexto(ListaEnlazada<Cancion>& lista, string nombreArchivo) {
    ifstream archivo(nombreArchivo);

    if (!archivo.is_open()) {
        cout << "Error: No se encontro el archivo " << nombreArchivo << endl;
        return;
    }

    string linea;
    while (getline(archivo, linea)) {

        
        //separar los 7 campos usando string stream
        stringstream ss(linea);
        string id_s, nombre, artista, album, anio_s, dur_s, ruta;
        
        getline(ss, id_s,    ',');
        getline(ss, nombre,  ',');
        getline(ss, artista, ',');
        getline(ss, album,   ',');
        getline(ss, anio_s,  ',');
        getline(ss, dur_s,   ',');
        getline(ss, ruta);

        // convertir de string to int
        int id       = stoi(id_s);
        int anio     = stoi(anio_s);
        int duracion = stoi(dur_s);

        // Limpiar el '\r' que deja Windows al final de la ultima columna
        //if (ruta.size() > 0 && ruta[ruta.size() - 1] == '\r') {
           // ruta = ruta.substr(0, ruta.size() - 1);
        //}

        // crear cancion e insertarla al final de la lista
        Cancion nueva(id, nombre, artista, album, anio, duracion, ruta);
        lista.insertarFinal(nueva);
    }

    archivo.close();
}


// guarda todas las canciones del catalogo en el archivo music_source.txt
void LectorArchivos::guardarCatalogo(ListaEnlazada<Cancion>& lista, string nombreArchivo) {
    ofstream archivo(nombreArchivo);

    if (!archivo.is_open()) {
        cout << "Error: No se pudo escribir en " << nombreArchivo << endl;
        return;
    }

    // recorrer la lista y escribir cada cancion en una linea
    Node<Cancion>* nodo = lista.getHead();
    while (nodo != nullptr) {
        Cancion c = nodo->data;
        archivo << c.id       << ","
                << c.nombre   << ","
                << c.artista  << ","
                << c.album    << ","
                << c.anio     << ","
                << c.duracion << ","
                << c.ruta     << "\n";
        nodo = nodo->next;
    }

    archivo.close();
}