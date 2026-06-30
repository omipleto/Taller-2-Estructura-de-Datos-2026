#include "MaxHeap.h"

HeapItem::HeapItem()
    : id(-1), primaryText(""), secondaryText(""), reproductions(0) {
}

HeapItem::HeapItem(int id, const std::string& primaryText,
                   const std::string& secondaryText, int reproductions)
    : id(id),
      primaryText(primaryText),
      secondaryText(secondaryText),
      reproductions(reproductions) {
}

MaxHeap::MaxHeap(int initialCapacity)
    : size(0), capacity(initialCapacity > 0 ? initialCapacity : 10) {
    items = new HeapItem[capacity];
}

MaxHeap::MaxHeap(const MaxHeap& other)
    : size(other.size), capacity(other.capacity) {
    items = new HeapItem[capacity];

    for (int i = 0; i < size; i++) {
        items[i] = other.items[i];
    }
}

MaxHeap::~MaxHeap() {
    delete[] items;
}

MaxHeap& MaxHeap::operator=(const MaxHeap& other) {
    if (this != &other) {
        delete[] items;

        size = other.size;
        capacity = other.capacity;
        items = new HeapItem[capacity];

        for (int i = 0; i < size; i++) {
            items[i] = other.items[i];
        }
    }

    return *this;
}

bool MaxHeap::hasHigherPriority(const HeapItem& a, const HeapItem& b) const {
    if (a.reproductions != b.reproductions) {
        return a.reproductions > b.reproductions;
    }

    if (a.primaryText != b.primaryText) {
        return a.primaryText < b.primaryText;
    }

    if (a.secondaryText != b.secondaryText) {
        return a.secondaryText < b.secondaryText;
    }

    return a.id < b.id;
}

void MaxHeap::swapItems(int a, int b) {
    HeapItem temp = items[a];
    items[a] = items[b];
    items[b] = temp;
}

void MaxHeap::heapifyUp(int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;

        if (!hasHigherPriority(items[index], items[parent])) {
            break;
        }

        swapItems(index, parent);
        index = parent;
    }
}

void MaxHeap::heapifyDown(int index) {
    while (true) {
        int left = index * 2 + 1;
        int right = index * 2 + 2;
        int largest = index;

        if (left < size && hasHigherPriority(items[left], items[largest])) {
            largest = left;
        }

        if (right < size && hasHigherPriority(items[right], items[largest])) {
            largest = right;
        }

        if (largest == index) {
            break;
        }

        swapItems(index, largest);
        index = largest;
    }
}

void MaxHeap::resize() {
    int newCapacity = capacity * 2;
    HeapItem* newItems = new HeapItem[newCapacity];

    for (int i = 0; i < size; i++) {
        newItems[i] = items[i];
    }

    delete[] items;
    items = newItems;
    capacity = newCapacity;
}

void MaxHeap::push(const HeapItem& item) {
    if (size == capacity) {
        resize();
    }

    items[size] = item;
    heapifyUp(size);
    size++;
}

HeapItem MaxHeap::top() const {
    if (isEmpty()) {
        return HeapItem();
    }

    return items[0];
}

HeapItem MaxHeap::pop() {
    if (isEmpty()) {
        return HeapItem();
    }

    HeapItem result = items[0];

    size--;

    if (size > 0) {
        items[0] = items[size];
        heapifyDown(0);
    }

    return result;
}

bool MaxHeap::isEmpty() const {
    return size == 0;
}

int MaxHeap::getSize() const {
    return size;
}

void MaxHeap::clear() {
    size = 0;
}