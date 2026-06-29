#include "Config.h"
#include <fstream>

Config::Config() : nextId(1), playing(false), shuffleMode(false), repeatMode(RepeatMode::OFF) {}

bool Config::loadFromFile(const std::string& filename, const LinkedList<Song>& allSongs) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    playlist.clear();
    
    std::string line;
    while (std::getline(file, line)) {
        size_t igualPos = line.find('=');
        if (igualPos == std::string::npos) continue;
        
        std::string key = line.substr(0, igualPos);
        std::string value = line.substr(igualPos + 1);
        
        if (key == "NEXT_ID") {
            nextId = std::stoi(value);
        }
        else if (key == "CURRENT_SONG_ID") {
            int songId = std::stoi(value);
            for (int i = 0; i < allSongs.getSize(); i++) {
                if (allSongs.get(i).getId() == songId) {
                    currentSong = allSongs.get(i);
                    break;
                }
            }
        }
        else if (key == "IS_PLAYING") {
            playing = (value == "true");
        }
        else if (key == "SHUFFLE_MODE") {
            shuffleMode = (value == "true");
        }
        else if (key == "REPEAT_MODE") {
            if (value == "OFF") repeatMode = RepeatMode::OFF;
            else if (value == "ONE") repeatMode = RepeatMode::ONE;
            else if (value == "ALL") repeatMode = RepeatMode::ALL;
        }
        else if (key == "PLAYLIST") {
            if (value.empty()) continue;
            
            size_t start = 0;
            size_t commaPos;
            do {
                commaPos = value.find(',', start);
                std::string idStr;
                if (commaPos == std::string::npos) {
                    idStr = value.substr(start);
                } else {
                    idStr = value.substr(start, commaPos - start);
                }
                
                if (!idStr.empty()) {
                    int songId = std::stoi(idStr);
                    for (int i = 0; i < allSongs.getSize(); i++) {
                        if (allSongs.get(i).getId() == songId) {
                            playlist.push_back(allSongs.get(i));
                            break;
                        }
                    }
                }
                start = commaPos + 1;
            } while (commaPos != std::string::npos);
        }
    }
    
    file.close();
    
    return true;
}

void Config::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) return;
    
    file << "NEXT_ID=" << nextId << "\n";
    file << "CURRENT_SONG_ID=" << (currentSong.isValid() ? currentSong.getId() : -1) << "\n";
    file << "IS_PLAYING=" << (playing ? "true" : "false") << "\n";
    file << "SHUFFLE_MODE=" << (shuffleMode ? "true" : "false") << "\n";
    
    std::string repeatStr;
    if (repeatMode == RepeatMode::OFF) repeatStr = "OFF";
    else if (repeatMode == RepeatMode::ONE) repeatStr = "ONE";
    else repeatStr = "ALL";
    file << "REPEAT_MODE=" << repeatStr << "\n";
    
    file << "PLAYLIST=";
    for (int i = 0; i < playlist.getSize(); i++) {
        if (i > 0) file << ",";
        file << playlist.get(i).getId();
    }
    file << "\n";
    
    file.close();
}

int Config::getNextId() const { return nextId; }
void Config::setNextId(int id) { nextId = id; }
Song Config::getCurrentSong() const { return currentSong; }
void Config::setCurrentSong(const Song& song) { currentSong = song; }
bool Config::isPlaying() const { return playing; }
void Config::setPlaying(bool p) { playing = p; }
bool Config::getShuffleMode() const { return shuffleMode; }
void Config::setShuffleMode(bool shuffle) { shuffleMode = shuffle; }
RepeatMode Config::getRepeatMode() const { return repeatMode; }
void Config::setRepeatMode(RepeatMode mode) { repeatMode = mode; }
LinkedList<Song>& Config::getPlaylist() { return playlist; }

void Config::setPlaylist(const LinkedList<Song>& newPlaylist) {
    playlist = newPlaylist;
}