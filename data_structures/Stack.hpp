#pragma once
#include "Node.hpp"

template <typename T>
class Stack {
private:
    Node<T>* top; 
    int size;

public:
    Stack() {
        top = nullptr;
        size = 0;
    }

    // agregar al historial
    void push(T value) {
        Node<T>* newNode = new Node<T>(value);
        newNode->next = top;
        top = newNode;
        size++;
    }

    // sacar la ultima cancion para volver atras
    T pop() {
        if (isEmpty()) return T(); // return vacio si no hay nada

        Node<T>* temp = top;
        T value = temp->data;
        top = top->next;
        delete temp; 
        size--;
        return value;
    }

    bool isEmpty() { return top == nullptr; }
    int getSize() { return size; }
};