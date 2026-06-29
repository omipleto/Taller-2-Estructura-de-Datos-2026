#include "FileManager.h"
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

FileManager::FileManager() {}

FileManager::~FileManager() {}


void FileManager::loadSongs(LinkedList<Song>& songs, const string& filename){
    ifstream archivo(filename);
    if (!archivo.is_open()) {
        cerr << "No se pudo abrir el archivo: " << filename << endl;
        return;
    }
    songs.clear();
    string linea;

    while (getline(archivo, linea)) {
        if (linea.empty()) continue;

        stringstream ss(linea);
        string idStr, title, artist, album, yearStr, durationStr, filePath;
        if (!getline(ss, idStr, ',')) continue;
        if (!getline(ss, title, ',')) continue;
        if (!getline(ss, artist, ',')) continue;
        if (!getline(ss, album, ',')) continue;
        if (!getline(ss, yearStr, ',')) continue;
        if (!getline(ss, durationStr, ',')) continue;
        if (!getline(ss, filePath, ',')) continue;

        if (idStr.empty() || yearStr.empty() || durationStr.empty()) continue;

        int id = 0;
        int year = 0;
        int duration = 0;
        try {
            id = stoi(idStr);
            year = stoi(yearStr);
            duration = stoi(durationStr);
        } catch (...) {
            continue;
        }

        Song song(id, title, artist, album, year, duration, filePath);
        if (song.isValid()) {
            songs.push_back(song);
        }
    }
    archivo.close();
}

void FileManager::saveSongs(LinkedList<Song>& songs, const string& filename){
    ofstream archivo(filename);
    if (!archivo.is_open()) {
        cerr << "No se pudo abrir el archivo: " << filename << endl;
        return;
    }

    for (int i = 0; i < songs.getSize(); i++) {
        archivo << songs.get(i).toFileLine() << "\n";
    }
    archivo.close();
}

void FileManager::addSong(const string& filename, const Song& song) {
    LinkedList<Song> songs;
    loadSongs(songs, filename);
    songs.push_back(song);
    saveSongs(songs, filename);
}

void FileManager::removeSong(const string& filename, int songId) {
    LinkedList<Song> songs;
    loadSongs(songs, filename);
    LinkedList<Song> updatedSongs;

    for (int i = 0; i < songs.getSize(); i++) {
        if (songs.get(i).getId() != songId) {
            updatedSongs.push_back(songs.get(i));
        }
    }
    saveSongs(updatedSongs, filename);
}