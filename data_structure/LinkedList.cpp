#include "LinkedList.h"

template<typename T>
Node<T>::Node(const T& d) : data(d), next(nullptr) {}

template<typename T>
LinkedList<T>::LinkedList() : head(nullptr), tail(nullptr), size(0) {
    srand(12345);
}

template<typename T>
LinkedList<T>::LinkedList(const LinkedList& other) : head(nullptr), tail(nullptr), size(0) {
    Node<T>* current = other.head;
    while (current) {
        push_back(current->data);
        current = current->next;
    }
}

template<typename T>
LinkedList<T>::~LinkedList() {
    clear();
}

template<typename T>
LinkedList<T>& LinkedList<T>::operator=(const LinkedList& other) {
    if (this != &other) {
        clear();
        Node<T>* current = other.head;
        while (current) {
            push_back(current->data);
            current = current->next;
        }
    }
    return *this;
}

template<typename T>
void LinkedList<T>::push_back(const T& data) {
    Node<T>* newNode = new Node<T>(data);
    if (!head) {
        head = tail = newNode;
    } else {
        tail->next = newNode;
        tail = newNode;
    }
    size++;
}

template<typename T>
void LinkedList<T>::push_front(const T& data) {
    Node<T>* newNode = new Node<T>(data);
    if (!head) {
        head = tail = newNode;
    } else {
        newNode->next = head;
        head = newNode;
    }
    size++;
}

template<typename T>
void LinkedList<T>::pop_front() {
    if (!head) return;
    Node<T>* temp = head;
    head = head->next;
    if (!head) tail = nullptr;
    delete temp;
    size--;
}

template<typename T>
void LinkedList<T>::pop_back() {
    if (!head) return;

    if (head == tail) {
        delete head;
        head = tail = nullptr;
    } else {
        Node<T>* current = head;
        while (current->next != tail) {
            current = current->next;
        }
        delete tail;
        tail = current;
        tail->next = nullptr;
    }
    size--;
}

template<typename T>
void LinkedList<T>::remove(int index) {
    if (index < 0 || index >= size) return;

    if (index == 0) {
        pop_front();
        return;
    }

    Node<T>* current = head;
    for (int i = 0; i < index - 1; i++) {
        current = current->next;
    }

    Node<T>* toDelete = current->next;
    current->next = toDelete->next;

    if (toDelete == tail) {
        tail = current;
    }

    delete toDelete;
    size--;
}

template<typename T>
T& LinkedList<T>::get(int index) {
    if (index < 0 || index >= size) {
        throw std::out_of_range("Indice fuera de rango");
    }
    Node<T>* current = head;
    for (int i = 0; i < index; i++) {
        current = current->next;
    }
    return current->data;
}

template<typename T>
const T& LinkedList<T>::get(int index) const {
    if (index < 0 || index >= size) {
        throw std::out_of_range("Indice fuera de rango");
    }
    Node<T>* current = head;
    for (int i = 0; i < index; i++) {
        current = current->next;
    }
    return current->data;
}

template<typename T>
int LinkedList<T>::getSize() const {
    return size;
}

template<typename T>
bool LinkedList<T>::isEmpty() const {
    return size == 0;
}

template<typename T>
void LinkedList<T>::clear() {
    while (head) {
        Node<T>* temp = head;
        head = head->next;
        delete temp;
    }
    tail = nullptr;
    size = 0;
}

template<typename T>
void LinkedList<T>::shuffle() {
    if (size <= 1) return;

    T* arr = new T[size];
    Node<T>* current = head;
    for (int i = 0; i < size; i++) {
        arr[i] = current->data;
        current = current->next;
    }

    for (int i = size - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        T temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }

    clear();
    for (int i = 0; i < size; i++) {
        push_back(arr[i]);
    }

    delete[] arr;
}

