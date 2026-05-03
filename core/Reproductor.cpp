#include "Reproductor.hpp"
#include "LectorArchivos.hpp"

// constructor
Reproductor::Reproductor() {
    reproduciendo = false;
    aleatorio     = false;
    repeticion    = REP_NINGUNA;
    srand(time(0)); // inicializamos el generador de numeros aleatorios
}


//mezclamos la cola aleatoriamente con el algoritmo fisher-yates
void Reproductor::mezclarCola() {
    int n = cola.getSize();
    if (n <= 1) return; // si la lista no tiene canciones, retornamos

    // copiamos las canciones a un arreglo temporal (de tipo cancion, para no perder informacion)
    Cancion* arr = new Cancion[n];
    Node<Cancion>* nodo = cola.getHead();
    for (int i = 0; i < n; i++) {
        arr[i] = nodo->data;
        nodo = nodo->next;
    }

    //usamos fisher yates recorriendo de fin a inicio
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);  // j es un indice al azar entre 0 e i 
        //(el modulo % siempre retorna un valor entre 0 y el elemento -1)
        
        Cancion temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }

    //vaciamos la cola y insertamos con la mezcla aleatoria
    cola.vaciar();
    for (int i = 0; i < n; i++) {
        cola.insertarFinal(arr[i]);
    }

    delete[] arr; // iberar la memoria del arreglo temp
}

//llenar la cola de reproduccion
void Reproductor::llenarCola() {
    cola.vaciar();

    // agregamos todas las canciones del music_source.txt a la cola
    Node<Cancion>* nodo = catalogo.getHead();
    while (nodo != nullptr) {
        if (nodo->data.id != actual.id) { // no agregar la cancion que está sonando actualmente
            cola.insertarFinal(nodo->data);
        }
        nodo = nodo->next;
    }

    // si estamos en modo aleatorio, mezclamos las canciones agregadas
    if (aleatorio) {
        mezclarCola();
    }
}


//opcion E: pista/cancion siguiente
void Reproductor::siguientePista() {
    if (!hayCanciones()) return;

    // si esta en modo (repetir una) no cambia la cancion
    if (repeticion == REP_UNA) {
        reproduciendo = true;
        return;
    }

    // guardar la cancion actual en el historial antes de avanzar ,historial existe en el hpp
    if (actual.id != 0) {
        historial.push(actual); 
    }
    

    // si la cola está vacía.. (no quedan canciones por reproducir)
    if (cola.getSize() == 0) {
        if (repeticion == REP_TODAS) {// repetir todas: recargar el catalogo en la cola y continuar
            llenarCola();
            if (cola.getSize() == 0) return; // no hay cancioens
        } else { 
			
            return;
			// en este punto ya no tenemos repeticion y no tenemos canciones en la cola
			//por lo tanto cortamos, para no seguir mirando hacia adelante (está vacio)
        }
    }

    // tomar la primera cancion de la cola como la actual
    actual = cola.eliminarPrimero();
    reproduciendo = true;
}


// pista anterior (Q)
void Reproductor::pistaAnterior() {
    if (!hayCanciones()) return;
    if (historial.isEmpty()) return;

    // si esta en modo (repetir una) no cambia la cancion
    if (repeticion == REP_UNA) {
        reproduciendo = true;
        return;
    }

    // agregamos la cancion actual a la cola (para no perderla)
    if (actual.id != 0) {
        cola.insertarInicio(actual);
    }

    // sacar la cancion anterior del historial y hacerla la actual
    actual = historial.pop();
    reproduciendo = true;
}

//modo aleatorio (S)
void Reproductor::activarDesactivarAleatorio() {
    if (!hayCanciones()) return;

    aleatorio = !aleatorio; // si es true-> false, y si es false-> true

    // si se activa, mezclar la cola
    if (aleatorio && cola.getSize() > 1) {
        mezclarCola();
    }
    // si se desactiva, la cola debe quedar igual
}


//modo de repeticion (R)
//modos: ninguna -> repetir una -> repetir todas -> inicio
void Reproductor::cambiarRepeticion() {
    if (!hayCanciones()) return;

    if (repeticion == REP_NINGUNA) {
        repeticion = REP_UNA;
    } else if (repeticion == REP_UNA) {
        repeticion = REP_TODAS;
    } else {
        repeticion = REP_NINGUNA;
    }
}

//agregar cancion al final de la cola (A<num del menu L)
void Reproductor::agregarACola(Cancion c) {
    cola.insertarFinal(c);
}


//saltar a posicion de la cola (S<num> del menu A)
void Reproductor::saltarACola(int pos) {

    // guardamos la cancion actual en el historial (hpp)
    if (actual.id != 0) {
        historial.push(actual); 
    }

    // eliminamos las canciones que estan antes de la posicion elegida
    for (int i = 0; i < pos - 1; i++) {
        cola.eliminarPrimero();
    }

    // la cancion en la posicion elegida sera la actual
    actual = cola.eliminarPrimero();
    reproduciendo = true;
}


//agregar nueva cancion al catalogo
void Reproductor::agregarCancion(Cancion c) {
    catalogo.insertarFinal(c);
}


//eliminar cancion del catalogo (archivo y en el programa (en la cola si es que estaba))
bool Reproductor::eliminarCancion(int idTarget) {
    // si la cancion que queremos eliminar es la que esta sonando, pasar a la siguiente
    if (actual.id == idTarget) {
        siguientePista();
        // si despues de avanzar sigue siendo la misma, detenemos el reproductor para evitar errores
        if (actual.id == idTarget) {
            actual = Cancion();
            reproduciendo = false;
        }
    }

    // eliminar la cancion del programa (cola)
    int n = cola.getSize();
    if (n > 0) {
        Cancion* temp = new Cancion[n];
        int nuevaSize = 0;
        Node<Cancion>* nodo = cola.getHead();
        while (nodo != nullptr) {
            if (nodo->data.id != idTarget) {
                temp[nuevaSize] = nodo->data;
                nuevaSize++;
            }
            nodo = nodo->next;
        }
        cola.vaciar();
        //agregamos la cola entera saltandonos esa cancion que estaba repetida (y que la queremeos eliminar)
        for (int i = 0; i < nuevaSize; i++) {
            cola.insertarFinal(temp[i]);
        }
        delete[] temp; //lib memoria
    }

    // eliminar del catalogo
    return catalogo.eliminarPorId(idTarget);
}

// ---------------------------------------------------------------

//string del estado actual de la reproducción
string Reproductor::getEstadoTexto() {
    if (actual.id == 0) return "Reproduccion Detenida";
    if (reproduciendo)  return "Reproduciendo";
    return "En Pausa";
}


//string del modo de repeticion actual
string Reproductor::getRepeticionTexto() {
    if (repeticion == REP_UNA)   return "R1";
    if (repeticion == REP_TODAS) return "RA";
    return "";
}

bool Reproductor::hayCanciones() {
    return catalogo.getSize() > 0;
}

// busca el id mas alto del catalogo
int Reproductor::maxIdCatalogo() {
    int maxId = 0;
    Node<Cancion>* nodo = catalogo.getHead();
    while (nodo != nullptr) {
        if (nodo->data.id > maxId) {
            maxId = nodo->data.id;
        }
        nodo = nodo->next;
    }
    return maxId;
}

// siguiente ID es el mas alto actual + 1 (si tengo 4 canciones, el proximo sera 5)
int Reproductor::proximoId() {
    return maxIdCatalogo() + 1;
}



// siempre se ejecuta: CARGAR estado de status.cfg
void Reproductor::cargarEstado(string archivo) {
    ifstream f(archivo);
    if (!f.is_open()) return; // no existe el archivo

    string linea;
    while (getline(f, linea)) { //while linea != ""
    
        //if (linea.empty()) continue; //ignoramos las lineas vacias

        // buscar la posicion de '=' con un for
        int posIgual = -1;
        for (int i = 0; i < (int)linea.size(); i++) {
            if (linea[i] == '=') {
                posIgual = i;
                break;
            }
        }

        // separar texto y valor usando substr
        string clave = linea.substr(0, posIgual);
        string valor = linea.substr(posIgual + 1);


        // asignar valores
        if (clave == "REPRODUCIENDO") {
        	if (valor == "1"){ //0 es pausa, 1 es reproduciendo
        		reproduciendo = true;
			}
            
        }
        else if (clave == "CANCION_ID") {
            int id = stoi(valor); 
            Node<Cancion>* nodo = catalogo.buscarPorId(id);
            if (nodo != nullptr) {
                actual = nodo->data; //guardamos el id de la cancion actual
            }
        }
        else if (clave == "ALEATORIO") {
        	if (valor == "1"){ //0 normal, 1 aleatorio
        		aleatorio = true;
			}
            
        }
        else if (clave == "REPETICION") {
            repeticion = stoi(valor); // guarda 0, 1 o 2
        }
        else if (clave == "COLA") {
            // la cola se guarda con id "1,2,3..."
            // la leemos separando por coma con un stringstream
            cola.vaciar();
            stringstream ss(valor); //valor aca vale lo que está a la derecha de "COLA=", puede ser "1,2,3,4.."
            string idStr;
            while (getline(ss, idStr, ',')) { //en idStr guardamos lo que encontramos antes de la coma
                int id = stoi(idStr); //aca lo guardamos como entero (string to int)
                Node<Cancion>* nodo = catalogo.buscarPorId(id);
                if (nodo != nullptr) {
                    cola.insertarFinal(nodo->data);
                }
            }
        }
    }

    f.close();
}


//GUARDAR el estado actual del reproductor en status.cfg
void Reproductor::guardarEstado(string archivo) {
    ofstream f(archivo);
    if (!f.is_open()) { //error, por alguna razon no podemos abrir el archivo 
        cout << "Error: No se pudo escribir " << archivo << endl;
        return;
    }

    // escribir cada variable en su propia linea
    if (reproduciendo) {
        f << "REPRODUCIENDO=1\n";
    } else {
        f << "REPRODUCIENDO=0\n";
    }

    f << "CANCION_ID=" << actual.id << "\n";

    if (aleatorio) {
        f << "ALEATORIO=1\n";
    } else {
        f << "ALEATORIO=0\n";
    }

    f << "REPETICION=" << repeticion << "\n"; // guardamos 0, 1 o 2

    
    f << "COLA=";
    Node<Cancion>* nodo = cola.getHead();
    bool primero = true;
    while (nodo != nullptr) {
        if (!primero) f << ","; //las comas solo las generamos después de escribir el primero, para que no quede ,1,2,3
        f << nodo->data.id;
        primero = false;
        nodo = nodo->next;
    }
    f << "\n";

    f.close();
}