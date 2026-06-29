
#include <string>
#include <vector>

template<typename T>
class AVLNode {
public:
    T data;
    AVLNode* left;
    AVLNode* right;
    int height;

    AVLNode(const T& d);
};

template<typename T>
class AVLTree {
private:
    AVLNode<T>* root;

    int getHeight(AVLNode<T>* node);
    int getBalance(AVLNode<T>* node);
    AVLNode<T>* rotateRight(AVLNode<T>* y);
    AVLNode<T>* rotateLeft(AVLNode<T>* x);
    AVLNode<T>* insert(AVLNode<T>* node, const T& data);
    void inOrder(AVLNode<T>* node, std::vector<T>& result);

public:
    AVLTree();
    ~AVLTree();

    void insert(const T& data);
    std::vector<T> inOrderTraversal();
    bool isEmpty() const;
    void clear();
    void clearNode(AVLNode<T>* node);
};

