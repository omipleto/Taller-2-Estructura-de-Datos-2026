#pragma once
#include <string>

struct HeapItem {
    int id;
    std::string primaryText;
    std::string secondaryText;
    int reproductions;

    HeapItem();
    HeapItem(int id, const std::string& primaryText,
             const std::string& secondaryText, int reproductions);
};

class MaxHeap {
private:
    HeapItem* items;
    int size;
    int capacity;

    bool hasHigherPriority(const HeapItem& a, const HeapItem& b) const;
    void swapItems(int a, int b);
    void heapifyUp(int index);
    void heapifyDown(int index);
    void resize();

public:
    MaxHeap(int initialCapacity = 10);
    MaxHeap(const MaxHeap& other);
    ~MaxHeap();

    MaxHeap& operator=(const MaxHeap& other);

    void push(const HeapItem& item);
    HeapItem top() const;
    HeapItem pop();

    bool isEmpty() const;
    int getSize() const;
    void clear();
};