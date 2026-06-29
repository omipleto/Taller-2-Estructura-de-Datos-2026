#include "Trie.h"
#include <queue>

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

Trie::Trie() {
    root = new TrieNode();
}

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

void Trie::insert(const std::string& word, int songId) {
    TrieNode* current = root;
    for (char c : word) {
        unsigned char idx = static_cast<unsigned char>(c);
        if (!current->children[idx]) {
            current->children[idx] = new TrieNode();
        }
        current = current->children[idx];
    }
    current->isEndOfWord = true;
    current->songId = songId;
}

std::vector<int> Trie::search(const std::string& prefix) {
    std::vector<int> results;
    TrieNode* current = root;

    // Buscar el nodo donde termina el prefijo
    for (char c : prefix) {
        unsigned char idx = static_cast<unsigned char>(c);
        if (!current->children[idx]) {
            return results; // No se encuentra el prefijo
        }
        current = current->children[idx];
    }

    // Recorrer todos los nodos hijos y agregar IDs de canciones
    std::queue<TrieNode*> q;
    q.push(current);

    while (!q.empty()) {
        TrieNode* node = q.front();
        q.pop();

        if (node->isEndOfWord && node->songId != -1) {
            results.push_back(node->songId);
        }

        for (int i = 0; i < 256; i++) {
            if (node->children[i]) {
                q.push(node->children[i]);
            }
        }
    }

    return results;
}

void Trie::remove(const std::string& word, int songId) {
    TrieNode* current = root;
    for (char c : word) {
        unsigned char idx = static_cast<unsigned char>(c);
        if (!current->children[idx]) {
            return;
        }
        current = current->children[idx];
    }
    if (current->isEndOfWord && current->songId == songId) {
        current->isEndOfWord = false;
        current->songId = -1;
    }
}

bool Trie::isEmpty() {
    for (int i = 0; i < 256; i++) {
        if (root->children[i]) {
            return false;
        }
    }
    return true;
}