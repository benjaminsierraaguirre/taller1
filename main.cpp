#include <iostream>
#include <string>
#include <fstream> //biblioteca archivos (leer, escribir)

//importamos todas las implementaciones (.hpp)
#include "classes/Cancion.hpp"
#include "data_structures/ListaEnlazada.hpp"
#include "data_structures/Stack.hpp"
#include "core/LectorArchivos.hpp"
#include "core/Reproductor.hpp"
#include "core/Menu.hpp"

using namespace std;

// función para extraer el numero (como r3, r2, s5)
int extraerNumero(string entrada) {
    if ((int)entrada.size() < 2) return -1;

    // verifico que todos los caracteres sean numeros
    for (int i = 1; i < (int)entrada.size(); i++) {
        if (!isdigit(entrada[i])) return -1;
    }

    return stoi(entrada.substr(1)); // funcion stoi para convertir de string a int
}


//lista de reproduccion actual (letra A)
void submenuCola(Reproductor& rep) {
    if (!rep.hayCanciones()) return;

    string entrada;
    bool salir = false;

    while (!salir) {
        Menu::mostrarCola(rep.actual, rep.cola, rep.aleatorio, rep.getRepeticionTexto());
        cin >> entrada;

        char cmd = toupper(entrada[0]);

        if (cmd == 'V') {
            salir = true;
        }
        else if (cmd == 'S' && rep.cola.getSize() > 0) {
            int num = extraerNumero(entrada); //obtenemos numero
            if (num >= 1 && num <= rep.cola.getSize()) {
                rep.saltarACola(num);
                rep.guardarEstado("status.cfg");
                salir = true; // volver al menu principal 
            } else {
                cout << "Numero invalido. Debe estar entre 1 y "
                     << rep.cola.getSize() << endl;
                cout << "Presione Enter para continuar..."; 
                cin.ignore();
                cin.get();
            }
        }
        else {
            cout << "Opcion no valida." << endl;
            cout << "Presione Enter para continuar...";
            cin.ignore();
            cin.get();
        }
    }
}


// lista de canciones en archivo (letra L)
void submenuCatalogo(Reproductor& rep) {
    string entrada;
    bool salir = false;

    while (!salir) {
        Menu::mostrarCatalogo(rep.actual, rep.catalogo, rep.aleatorio, rep.getRepeticionTexto());
        cin >> entrada;

        char cmd = toupper(entrada[0]);
        int num  = extraerNumero(entrada);

        // R<num>: reproducir la cancion elegida
        if (cmd == 'R' && num >= 1) {
            if (num > rep.catalogo.getSize()) {
                cout << "Numero invalido." << endl;
                cout << "Presione Enter para continuar...";
                cin.ignore(); 
				cin.get();
            }   
            else {
            	// guardar la cancion actual en el historial
            if (rep.actual.id != 0) {
                rep.historial.push(rep.actual);
            }
            // obtener la cancion elegida por su posicion en la lista 
            Node<Cancion>* nodo = rep.catalogo.getNodeAt(num - 1);
            rep.actual = nodo->data;
            rep.reproduciendo = true;

            // recargar la cola con el resto de canciones
            rep.llenarCola();
            rep.guardarEstado("status.cfg");
            salir = true;
            	
			}
			
            
        }
        // A<num>: agregar cancion al final de la cola
        else if (cmd == 'A' && num >= 1) {
            if (num > rep.catalogo.getSize()) {
                cout << "Numero invalido." << endl;
                cout << "Presione Enter para continuar...";
                cin.ignore(); 
				cin.get();
                
            }
            else {
	            Node<Cancion>* nodo = rep.catalogo.getNodeAt(num - 1);
	            rep.agregarACola(nodo->data);
	            rep.guardarEstado("status.cfg");
	            cout << nodo->data.nombre << " agregada al final de la cola." << endl;
	            cout << "Presione Enter para continuar...";
	            cin.ignore(); 
				cin.get();	
			}
            
        }
        // N: agregar nueva cancion al archivo/registro
        else if (cmd == 'N') {
            Cancion nueva = Menu::pedirDatosCancion(rep.proximoId());
            rep.agregarCancion(nueva);
            LectorArchivos::guardarCatalogo(rep.catalogo, "music_source.txt");
            rep.guardarEstado("status.cfg");
            cout << nueva.nombre << " agregada al catalogo." << endl;
            cout << "Presione Enter para continuar...";
            cin.ignore(); 
			cin.get();
        }
        
        // D<num>: eliminar cancion del archivo/registro
        else if (cmd == 'D' && num >= 1) {
            if (num > rep.catalogo.getSize()) {
                cout << "Numero invalido." << endl;
                cout << "Presione Enter para continuar...";
                cin.ignore(); 
				cin.get();
            }
            else {
            	Node<Cancion>* nodo = rep.catalogo.getNodeAt(num - 1);
	            string nombreBorrado = nodo->data.nombre;
	            int idBorrado = nodo->data.id;
	
	            rep.eliminarCancion(idBorrado);
	            LectorArchivos::guardarCatalogo(rep.catalogo, "music_source.txt");
	            rep.guardarEstado("status.cfg");
	            cout << nombreBorrado << " eliminada." << endl;
	            cout << "Presione Enter para continuar...";
	            cin.ignore(); 
				cin.get();
			}
            
        }
        // V: volver al menu
        else if (cmd == 'V') {
            salir = true;
        }
        else {
            cout << "Opcion no valida." << endl;
            cout << "Presione Enter para continuar...";
            cin.ignore(); 
			cin.get();
        }
    }
}

//main
int main() {
    Reproductor rep;

    // 1 - cargar canciones desde el archivo
    LectorArchivos::cargarDesdeTexto(rep.catalogo, "music_source.txt");

    // 2- cargar el estado previo desde status.cfg 
    //    solo si hay canciones disponibles
    if (rep.catalogo.getSize() > 0) {
        rep.cargarEstado("status.cfg");
    }

    // 3- si status.cfg todavia no existe, crearlo con valores por defecto
    ifstream test("status.cfg");
    if (!test.is_open()) {
        rep.guardarEstado("status.cfg");
    }
    test.close();

    // 4 - bucle principal
    string entrada;
    bool salir = false;

    while (!salir) {
        Menu::mostrarPrincipal(rep.actual, rep.getEstadoTexto(), rep.aleatorio, rep.getRepeticionTexto(),!rep.historial.isEmpty()); 
        //!rep.historial.isEmpty() es true cuando SI hay canciones en el historial
        //ya que preguntamos: está vacio?, no preguntamos: tiene canciones?

        cin >> entrada;

        char cmd = toupper(entrada[0]);

        switch (cmd) {

            case 'W':  //reproducir o pausar
                if (rep.hayCanciones()) {
                	if (rep.actual.id == 0) { // no hay cancion actual: tomar la primera de la cola
	                    rep.siguientePista();
	                } else {
	                    rep.reproduciendo = !rep.reproduciendo; // si reproduciendo -> pausa, y si pausa -> reproduciendo
	                }
	                rep.guardarEstado("status.cfg");
				}
                break;

            case 'Q':  // pista anterior
                if (rep.historial.isEmpty()) break; // si no tenemos historial, no podemos ejecutarlo
                rep.pistaAnterior();
                rep.guardarEstado("status.cfg");
                break;

            case 'E':  // pista sigueinte
                if (rep.hayCanciones()) { //solo si existen canciones en el catalogo
	                if (rep.cola.getSize() == 0) { //si no hay cola la llenamos primero
	                    rep.llenarCola();
	                }
	                rep.siguientePista();
	                rep.guardarEstado("status.cfg");
				}
                break;

            case 'S':  // modo aleatorio on/off
                rep.activarDesactivarAleatorio();
                rep.guardarEstado("status.cfg");
                break;

            case 'R':  // modo de repeticion
                rep.cambiarRepeticion();
                rep.guardarEstado("status.cfg");
                break;

            case 'A':  // cola/lista de reproduccion actual
            	if (rep.hayCanciones()){
            		submenuCola(rep);
				}
                break;

            case 'L':  // catalogo de canciones (del archivo)
                submenuCatalogo(rep);
                break;

            case 'X':  // salir
                rep.guardarEstado("status.cfg");
                cout << "Estado guardado. Hasta luego!" << endl;
                salir = true;
                break;

            default:
                //opcion invalida: hacemos nada y redibujamos el menu
                break;
        }
    }

    return 0;
}