#pragma once
#include <cstdlib>
#include <stdexcept>

template<typename T>
class Node {
public:
    T data;
    Node<T>* next;
    Node(const T& d);
};

template<typename T>
class LinkedList {
private:
    Node<T>* head;
    Node<T>* tail;
    int size;

public:
    LinkedList();
    LinkedList(const LinkedList& other);
    ~LinkedList();

    LinkedList& operator=(const LinkedList& other);

    void push_back(const T& data);
    void push_front(const T& data);
    void pop_front();
    void pop_back();
    void remove(int index);
    T& get(int index);
    const T& get(int index) const;
    int getSize() const;
    bool isEmpty() const;
    void clear();
    void shuffle();
};

