#pragma once
#include <string>
#include <vector>

class TrieNode {
public:
    TrieNode* children[256]; 
    bool isEndOfWord;
    int songId;

    TrieNode();
    ~TrieNode();
};

class Trie {
private:
    TrieNode* root;
    int nodeCount;
    void clearNode(TrieNode* node);

public:
    Trie();
    ~Trie();

    void insert(const std::string& word, int songId);
    std::vector<int> search(const std::string& prefix);
    void remove(const std::string& word, int songId);
    bool isEmpty();
    void clear();
};
