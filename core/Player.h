#pragma once
#include "../data_structures/LinkedList.h"
#include "../models/Song.h"
#include "../models/Config.h"
#include "FileManager.h"
#include <string>

class Player {
private:
    LinkedList<Song> allSongs;
    Config config;
    LinkedList<Song> history;
    FileManager fileManager;
    
    void updateConfigFile();
    void regeneratePlaylist();
    void addToHistory(const Song& song);
    Song getPreviousFromHistory();
    void clearScreen();
    std::string getRepeatModeString() const;
    std::string getStatusPrefix() const;
    
public:
    Player();
    ~Player();
    
    bool initialize();
    void run();
    
    void playPause();
    void previousTrack();
    void nextTrack();
    void toggleShuffle();
    void toggleRepeat();
    void showCurrentPlaylist();
    void showAllSongsMenu();
    void exit();
};

