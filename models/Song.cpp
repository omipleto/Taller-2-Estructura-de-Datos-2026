#include "Song.h"
#include <iostream>
#include <sstream>

Song::Song() : id(-1), year(0), durationSeconds(0) {}

Song::Song(int id, const std::string& title, const std::string& artist,
           const std::string& album, int year, int durationSeconds, const std::string& filePath)
    : id(id), title(title), artist(artist), album(album), 
      year(year), durationSeconds(durationSeconds), filePath(filePath) {}

int Song::getId() const { return id; }
std::string Song::getTitle() const { return title; }
std::string Song::getArtist() const { return artist; }
std::string Song::getAlbum() const { return album; }
int Song::getYear() const { return year; }
int Song::getDurationSeconds() const { return durationSeconds; }
std::string Song::getFilePath() const { return filePath; }

void Song::show(){
    std::cout << this->id << " - " << this->title << " - " << this->artist << std::endl;
}

std::string Song::toFileLine() const {
    std::stringstream ss;
    ss << id << "," << title << "," << artist << "," << album << ","
       << year << "," << durationSeconds << "," << filePath;
    return ss.str();
}

bool Song::isValid() const {
    return id != -1 && !title.empty() && !artist.empty();
}