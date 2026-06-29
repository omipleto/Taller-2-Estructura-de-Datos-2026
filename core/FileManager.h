#pragma once
#include "../data_structures/LinkedList.h"
#include "../models/Song.h"
#include <string>

class FileManager {
public:
    FileManager();
    ~FileManager();

    void loadSongs(LinkedList<Song>& songs, const std::string& filename);
    void saveSongs(LinkedList<Song>& songs, const std::string& filename);
    void addSong(const std::string& filename, const Song& song);
    void removeSong(const std::string& filename, int songId);
};    

