#pragma once
#include "Node.hpp"
#include <iostream>

using namespace std;

template <typename T>
class ListaEnlazada {
private:
    Node<T>* head; 
    int size;      

public:
    ListaEnlazada() { //constructor
        head = nullptr;
        size = 0;
    }

    // destructor
    ~ListaEnlazada() {
        Node<T>* actual = head;
        while (actual != nullptr) {
            Node<T>* siguiente = actual->next;
            delete actual;
            actual = siguiente;
        }
    }

    // insertar al final para cargar music_source.txt en orden
    void insertarFinal(T data) {
        Node<T>* nuevo = new Node<T>(data);
        if (head == nullptr) {
            head = nuevo;
        } else {
            Node<T>* temp = head;
            while (temp->next != nullptr) {
                temp = temp->next;
            }
            temp->next = nuevo;
        }
        size++;
    }

    // insertar al inicio
    void insertarInicio(T data) {
        Node<T>* nuevo = new Node<T>(data);
        nuevo->next = head;
        head = nuevo;
        size++;
    }

    // obtener nodo por posicion, retornamos el nodo
    Node<T>* getNodeAt(int pos) {
        if (pos < 0 || pos >= size) return nullptr; //caso base
        Node<T>* actual = head;
        for (int i = 0; i < pos; i++) {
            actual = actual->next;
        }
        return actual;
    }

    // obtener tamaño
    int getSize() {
        return size;
    }

    // obtener el primer nodo
    Node<T>* getHead() {
        return head;
    }

    // vaciar la lista para liberar memoria
    void vaciar() {
        Node<T>* actual = head;
        while (actual != nullptr) {
            Node<T>* siguiente = actual->next;
            delete actual;
            actual = siguiente;
        }
        head = nullptr;
        size = 0;
    }

    // eliminar el primer nodo y retornar su dato
    T eliminarPrimero() {
        if (head == nullptr) return T();
        Node<T>* aBorrar = head;
        T valor = aBorrar->data;
        head = head->next;
        delete aBorrar;
        size--;
        return valor;
    }

    //eliminar por id
    bool eliminarPorId(int idTarget) {
        if (head == nullptr) return false;

        // si el nodo es la cabeza, muevo head al segundo y borro el primero
        if (head->data.id == idTarget) {
            Node<T>* aBorrar = head;
            head = head->next;
            delete aBorrar;
            size--;
            return true;
        }

        // buscar en el resto de la lista
        Node<T>* actual = head;
        while (actual->next != nullptr && actual->next->data.id != idTarget) {
            actual = actual->next; 
        }

        if (actual->next != nullptr) {
            Node<T>* aBorrar = actual->next;
            actual->next = aBorrar->next;
            delete aBorrar;
            size--;
            return true;
        }
        return false;
    }

    // eliminar nodo en posición dada 
    bool eliminarEnPosicion(int pos) {
        if (pos < 0 || pos >= size || head == nullptr) return false;

        if (pos == 0) {
            Node<T>* aBorrar = head;
            head = head->next;
            delete aBorrar;
            size--;
            return true;
        }

        Node<T>* actual = head;
        for (int i = 0; i < pos - 1; i++) {
            actual = actual->next;
        }
        Node<T>* aBorrar = actual->next;
        actual->next = aBorrar->next;
        delete aBorrar;
        size--;
        return true;
    }

    // buscar por id y retornar el nodo (nullptr si no existe)
    Node<T>* buscarPorId(int idTarget) {
        Node<T>* actual = head;
        while (actual != nullptr) {
            if (actual->data.id == idTarget) return actual;
            actual = actual->next;
        }
        return nullptr;
    }
};