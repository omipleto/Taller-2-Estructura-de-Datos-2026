#include "Ranking.h"
#include <fstream>
#include <sstream>

using namespace std;

Ranking::Ranking(int initialCapacity)
    : size(0), capacity(initialCapacity > 0 ? initialCapacity : 10) {
    records = new RankingRecord[capacity];
}

Ranking::Ranking(const Ranking& other)
    : size(other.size), capacity(other.capacity) {
    records = new RankingRecord[capacity];

    for (int i = 0; i < size; i++) {
        records[i] = other.records[i];
    }
}

Ranking::~Ranking() {
    delete[] records;
}

Ranking& Ranking::operator=(const Ranking& other) {
    if (this != &other) {
        delete[] records;

        size = other.size;
        capacity = other.capacity;
        records = new RankingRecord[capacity];

        for (int i = 0; i < size; i++) {
            records[i] = other.records[i];
        }
    }

    return *this;
}

int Ranking::findIndex(int songId) const {
    for (int i = 0; i < size; i++) {
        if (records[i].songId == songId) {
            return i;
        }
    }

    return -1;
}

void Ranking::resize() {
    int newCapacity = capacity * 2;
    RankingRecord* newRecords = new RankingRecord[newCapacity];

    for (int i = 0; i < size; i++) {
        newRecords[i] = records[i];
    }

    delete[] records;
    records = newRecords;
    capacity = newCapacity;
}

void Ranking::loadFromFile(const string& filename) {
    ifstream file(filename);

    if (!file.is_open()) {
        return;
    }

    clear();

    string line;

    while (getline(file, line)) {
        if (line.empty()) {
            continue;
        }

        stringstream ss(line);
        string idStr;
        string reproductionsStr;

        if (!getline(ss, idStr, ',')) {
            continue;
        }

        if (!getline(ss, reproductionsStr, ',')) {
            continue;
        }

        try {
            int songId = stoi(idStr);
            int reproductions = stoi(reproductionsStr);

            if (size == capacity) {
                resize();
            }

            records[size].songId = songId;
            records[size].reproductions = reproductions;
            size++;
        }
        catch (...) {
            continue;
        }
    }

    file.close();
}

void Ranking::saveToFile(const string& filename) const {
    ofstream file(filename);

    if (!file.is_open()) {
        return;
    }

    for (int i = 0; i < size; i++) {
        file << records[i].songId << ","
             << records[i].reproductions << endl;
    }

    file.close();
}

void Ranking::registerPlay(int songId) {
    int index = findIndex(songId);

    if (index != -1) {
        records[index].reproductions++;
        return;
    }

    if (size == capacity) {
        resize();
    }

    records[size].songId = songId;
    records[size].reproductions = 1;
    size++;
}

int Ranking::getPlayCount(int songId) const {
    int index = findIndex(songId);

    if (index == -1) {
        return 0;
    }

    return records[index].reproductions;
}

void Ranking::removeSong(int songId) {
    int index = findIndex(songId);

    if (index == -1) {
        return;
    }

    for (int i = index; i < size - 1; i++) {
        records[i] = records[i + 1];
    }

    size--;
}

void Ranking::clear() {
    size = 0;
}

MaxHeap Ranking::buildSongHeap(const LinkedList<Song>& songs) const {
    MaxHeap heap;

    for (int i = 0; i < songs.getSize(); i++) {
        Song song = songs.get(i);

        HeapItem item(
            song.getId(),
            song.getTitle(),
            song.getArtist(),
            getPlayCount(song.getId())
        );

        heap.push(item);
    }

    return heap;
}

MaxHeap Ranking::buildArtistHeap(const LinkedList<Song>& songs) const {
    MaxHeap heap;

    int songCount = songs.getSize();

    if (songCount == 0) {
        return heap;
    }

    string* artists = new string[songCount];
    int* reproductions = new int[songCount];
    int artistCount = 0;

    for (int i = 0; i < songCount; i++) {
        Song song = songs.get(i);
        string artist = song.getArtist();
        int artistIndex = -1;

        for (int j = 0; j < artistCount; j++) {
            if (artists[j] == artist) {
                artistIndex = j;
                break;
            }
        }

        if (artistIndex == -1) {
            artists[artistCount] = artist;
            reproductions[artistCount] = getPlayCount(song.getId());
            artistCount++;
        }
        else {
            reproductions[artistIndex] += getPlayCount(song.getId());
        }
    }

    for (int i = 0; i < artistCount; i++) {
        HeapItem item(
            -1,
            artists[i],
            "",
            reproductions[i]
        );

        heap.push(item);
    }

    delete[] artists;
    delete[] reproductions;

    return heap;
}