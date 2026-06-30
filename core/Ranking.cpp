#pragma once

#include "../data_structure/MaxHeap.h"
#include "../data_structure/LinkedList.h"
#include "../models/Song.h"
#include <string>

struct RankingRecord {
    int songId;
    int reproductions;
};

class Ranking {
private:
    RankingRecord* records;
    int size;
    int capacity;

    int findIndex(int songId) const;
    void resize();

public:
    Ranking(int initialCapacity = 10);
    Ranking(const Ranking& other);
    ~Ranking();

    Ranking& operator=(const Ranking& other);

    void loadFromFile(const std::string& filename);
    void saveToFile(const std::string& filename) const;

    void registerPlay(int songId);
    int getPlayCount(int songId) const;
    void removeSong(int songId);

    void clear();

    MaxHeap buildSongHeap(const LinkedList<Song>& songs) const;
    MaxHeap buildArtistHeap(const LinkedList<Song>& songs) const;
};