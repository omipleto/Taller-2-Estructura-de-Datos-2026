
#include "AVLTree.h"
#include <algorithm>

template<typename T>
AVLNode<T>::AVLNode(const T& d) : data(d), left(nullptr), right(nullptr), height(1) {}

template<typename T>
AVLTree<T>::AVLTree() : root(nullptr) {}

template<typename T>
AVLTree<T>::~AVLTree() {
    clear();
}

template<typename T>
int AVLTree<T>::getHeight(AVLNode<T>* node) {
    return node ? node->height : 0;
}

template<typename T>
int AVLTree<T>::getBalance(AVLNode<T>* node) {
    return node ? getHeight(node->left) - getHeight(node->right) : 0;
}

template<typename T>
AVLNode<T>* AVLTree<T>::rotateRight(AVLNode<T>* y) {
    AVLNode<T>* x = y->left;
    AVLNode<T>* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = std::max(getHeight(y->left), getHeight(y->right)) + 1;
    x->height = std::max(getHeight(x->left), getHeight(x->right)) + 1;

    return x;
}

template<typename T>
AVLNode<T>* AVLTree<T>::rotateLeft(AVLNode<T>* x) {
    AVLNode<T>* y = x->right;
    AVLNode<T>* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = std::max(getHeight(x->left), getHeight(x->right)) + 1;
    y->height = std::max(getHeight(y->left), getHeight(y->right)) + 1;

    return y;
}

template<typename T>
AVLNode<T>* AVLTree<T>::insert(AVLNode<T>* node, const T& data) {
    if (!node) {
        return new AVLNode<T>(data);
    }

    if (data < node->data) {
        node->left = insert(node->left, data);
    } else if (data > node->data) {
        node->right = insert(node->right, data);
    } else {
        return node; // Duplicado, no insertar
    }

    node->height = std::max(getHeight(node->left), getHeight(node->right)) + 1;

    int balance = getBalance(node);

    // Rotación LL
    if (balance > 1 && data < node->left->data) {
        return rotateRight(node);
    }

    // Rotación RR
    if (balance < -1 && data > node->right->data) {
        return rotateLeft(node);
    }

    // Rotación LR
    if (balance > 1 && data > node->left->data) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }

    // Rotación RL
    if (balance < -1 && data < node->right->data) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node;
}

template<typename T>
void AVLTree<T>::insert(const T& data) {
    root = insert(root, data);
}

template<typename T>
void AVLTree<T>::inOrder(AVLNode<T>* node, std::vector<T>& result) {
    if (node) {
        inOrder(node->left, result);
        result.push_back(node->data);
        inOrder(node->right, result);
    }
}

template<typename T>
std::vector<T> AVLTree<T>::inOrderTraversal() {
    std::vector<T> result;
    inOrder(root, result);
    return result;
}

template<typename T>
bool AVLTree<T>::isEmpty() const {
    return root == nullptr;
}

template<typename T>
void AVLTree<T>::clearNode(AVLNode<T>* node) {
    if (node) {
        clearNode(node->left);
        clearNode(node->right);
        delete node;
    }
}

template<typename T>
void AVLTree<T>::clear() {
    clearNode(root);
    root = nullptr;
}

