#pragma once

template <typename T>
class Node {
public:
    T data;         // el objeto que guardamos 
    Node<T>* next;  

    Node(T value) {
        data = value;
        next = nullptr;
    }
};