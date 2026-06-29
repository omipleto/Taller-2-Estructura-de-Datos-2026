#pragma once
#include "../data_structures/LinkedList.h"
#include "Song.h"
#include <string>

enum class RepeatMode {
    OFF,
    ONE,
    ALL
};

class Config {
private:
    int nextId;
    Song currentSong;
    bool playing;
    bool shuffleMode;
    RepeatMode repeatMode;
    LinkedList<Song> playlist;
    
public:
    Config();
    
    bool loadFromFile(const std::string& filename, const LinkedList<Song>& allSongs);
    void saveToFile(const std::string& filename) const;
    
    int getNextId() const;
    void setNextId(int id);
    Song getCurrentSong() const;
    void setCurrentSong(const Song& song);
    bool isPlaying() const;
    void setPlaying(bool p);
    bool getShuffleMode() const;
    void setShuffleMode(bool shuffle);
    RepeatMode getRepeatMode() const;
    void setRepeatMode(RepeatMode mode);
    LinkedList<Song>& getPlaylist();
    void setPlaylist(const LinkedList<Song>& newPlaylist);
};