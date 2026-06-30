#pragma once
#include "../data_structure/LinkedList.h"
#include "../models/Song.h"
#include "../models/Config.h"
#include "FileManager.h"
#include "Ranking.h"
#include <string>

class Player {
private:
    LinkedList<Song> allSongs;
    Config config;
    LinkedList<Song> history;
    FileManager fileManager;
    Ranking ranking;

    void updateConfigFile();
    void updateRankingFile();
    void regeneratePlaylist();
    void addToHistory(const Song& song);
    Song getPreviousFromHistory();
    Song getSongById(int songId) const;
    void clearScreen();
    std::string getRepeatModeString() const;
    std::string getStatusPrefix() const;

    void registerPlay(const Song& song);
    void playSelectedSong(const Song& song);

    void showTopMenu();
    void showTopSongs();
    void showTopArtists();
    bool showArtistSongs(const std::string& artist);

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