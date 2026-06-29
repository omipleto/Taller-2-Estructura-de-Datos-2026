#include "Trie.h"
#include <vector>

using namespace std;



TrieNode::TrieNode() : isEndOfWord(false), songId(-1) {
    for (int i = 0; i < 256; i++) {
        children[i] = nullptr;
    }
}

TrieNode::~TrieNode() {
    for (int i = 0; i < 256; i++) {
        if (children[i]) {
            delete children[i];
        }
    }
}


Trie::Trie() : root(new TrieNode()), nodeCount(1) {}

Trie::~Trie() {
    delete root;
}

void Trie::clearNode(TrieNode* node) {
    if (!node) return;
    for (int i = 0; i < 256; i++) {
        if (node->children[i]) {
            clearNode(node->children[i]);
        }
    }
    delete node;
}

void Trie::clear() {
    clearNode(root);
    root = new TrieNode();
    nodeCount = 1;
}

void Trie::insert(const std::string& word, int songId) {
    TrieNode* current = root;
    for (char c : word) {
        unsigned char idx = static_cast<unsigned char>(c);
        if (!current->children[idx]) {
            current->children[idx] = new TrieNode();
            nodeCount++;
        }
        current = current->children[idx];
    }
    current->isEndOfWord = true;
    current->songId = songId;
}

void Trie::collectWords(TrieNode* node, vector<int>& results) {
    if (!node) return;

    // Si este nodo es final de palabra, agregar el ID
    if (node->isEndOfWord && node->songId != -1) {
        results.push_back(node->songId);
    }

    // Recorrer todos los hijos recursivamente (DFS)
    for (int i = 0; i < 256; i++) {
        if (node->children[i]) {
            collectWords(node->children[i], results);
        }
    }
}

std::vector<int> Trie::search(const std::string& prefix) {
    vector<int> results;
    TrieNode* current = root;

    // Buscar el nodo donde termina el prefijo
    for (char c : prefix) {
        unsigned char idx = static_cast<unsigned char>(c);
        if (!current->children[idx]) {
            return results; // No se encuentra el prefijo
        }
        current = current->children[idx];
    }

    // Recorrer desde el nodo encontrado
    collectWords(current, results);

    return results;
}

void Trie::remove(const std::string& word, int songId) {
    TrieNode* current = root;
    vector<TrieNode*> path; // Para posible eliminación de nodos huérfanos

    for (char c : word) {
        unsigned char idx = static_cast<unsigned char>(c);
        if (!current->children[idx]) {
            return; // La palabra no existe
        }
        path.push_back(current);
        current = current->children[idx];
    }

   
    if (current->isEndOfWord && current->songId == songId) {
        current->isEndOfWord = false;
        current->songId = -1;

        
    }
}

bool Trie::isEmpty() const {
    for (int i = 0; i < 256; i++) {
        if (root->children[i]) {
            return false;
        }
    }
    return true;
}