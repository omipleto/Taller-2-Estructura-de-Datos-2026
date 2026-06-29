#include "Player.h"
#include <iostream>
#include <limits>
#include <cstdlib>

using namespace std;

Player::Player() {}

bool Player::initialize() {
    fileManager.loadSongs(allSongs, "music_source.txt");
    
    if (!config.loadFromFile("status.cfg", allSongs)) {
        if (!allSongs.isEmpty()) {
            regeneratePlaylist();
            if (!config.getPlaylist().isEmpty()) {
                Song firstSong = config.getPlaylist().get(0);
                config.getPlaylist().pop_front();
                config.setCurrentSong(firstSong);
            }
            config.setPlaying(false);
            config.setShuffleMode(false);
            config.setRepeatMode(RepeatMode::OFF);
            config.setNextId(allSongs.getSize() + 1);
        }
        updateConfigFile();
    }
    
    return true;
}

void Player::run() {
    char option;
    bool running = true;
    
    while (running) {
        clearScreen();
        
        cout << "Actual";
        string estado = getStatusPrefix();
        if (!estado.empty()) {
            cout << " (" << estado << ")";
        }
        if (config.getCurrentSong().isValid()) {
            cout << ": " << config.getCurrentSong().getTitle() << " - " << config.getCurrentSong().getArtist();
        } else {
            cout << ": Reproduccion detenida";
        }
        cout << "\n\n";
        
        cout << "Opciones:\n";
        cout << "W - Reproducir/Pausar\n";
        cout << "Q - Pista Anterior\n";
        cout << "E - Pista Siguiente\n";
        cout << "S - Activar/Desactivar modo aleatorio\n";
        cout << "R - Repeticion (Desactivado/Repetir una/Repetir todas)\n";
        cout << "A - Ver lista de reproduccion actual\n";
        cout << "L - Listado de canciones\n";
        cout << "X - Salir\n";
        cout << "Ingrese Opcion: ";
        
        cin >> option;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        switch (option) {
            case 'W': case 'w': playPause(); break;
            case 'Q': case 'q': previousTrack(); break;
            case 'E': case 'e': nextTrack(); break;
            case 'S': case 's': toggleShuffle(); break;
            case 'R': case 'r': toggleRepeat(); break;
            case 'A': case 'a': showCurrentPlaylist(); break;
            case 'L': case 'l': showAllSongsMenu(); break;
            case 'X': case 'x': 
                exit();
                running = false;
                break;
            default:
                cout << "Opcion invalida\n";
                cin.get();
        }
    }
}

void Player::playPause() {
    if (allSongs.isEmpty()) return;
    config.setPlaying(!config.isPlaying());
    updateConfigFile();
}

void Player::previousTrack() {
    if (allSongs.isEmpty()) return;
    
    Song previous = getPreviousFromHistory();
    if (previous.isValid()) {
        if (config.getCurrentSong().isValid()) {
            addToHistory(config.getCurrentSong());
        }
        config.setCurrentSong(previous);
        config.setPlaying(true);
    }
    updateConfigFile();
}

void Player::nextTrack() {
    if (allSongs.isEmpty()) return;
    
    if (config.getCurrentSong().isValid()) {
        addToHistory(config.getCurrentSong());
    }
    
    if (!config.getPlaylist().isEmpty()) {
        Song nextSong = config.getPlaylist().get(0);
        config.getPlaylist().pop_front();
        config.setCurrentSong(nextSong);
        config.setPlaying(true);
    }
    else if (!allSongs.isEmpty()) {
        if (config.getRepeatMode() == RepeatMode::ALL) {
            regeneratePlaylist();
            if (!config.getPlaylist().isEmpty()) {
                Song nextSong = config.getPlaylist().get(0);
                config.getPlaylist().pop_front();
                config.setCurrentSong(nextSong);
                config.setPlaying(true);
            }
        } else if (config.getRepeatMode() == RepeatMode::ONE) {
            config.setPlaying(true);
        } else {
            regeneratePlaylist();
            if (!config.getPlaylist().isEmpty()) {
                Song nextSong = config.getPlaylist().get(0);
                config.getPlaylist().pop_front();
                config.setCurrentSong(nextSong);
                config.setPlaying(true);
            }
        }
    }
    
    updateConfigFile();
}

void Player::toggleShuffle() {
    if (allSongs.isEmpty()) return;
    config.setShuffleMode(!config.getShuffleMode());
    if (config.getShuffleMode()) {
        config.getPlaylist().shuffle();
    }
    updateConfigFile();
}

void Player::toggleRepeat() {
    if (allSongs.isEmpty()) return;
    
    switch (config.getRepeatMode()) {
        case RepeatMode::OFF:
            config.setRepeatMode(RepeatMode::ONE);
            break;
        case RepeatMode::ONE:
            config.setRepeatMode(RepeatMode::ALL);
            break;
        case RepeatMode::ALL:
            config.setRepeatMode(RepeatMode::OFF);
            break;
    }
    updateConfigFile();
}

void Player::showCurrentPlaylist() {
    if (allSongs.isEmpty()) return;
    
    bool inSubmenu = true;
    string option;
    
    while (inSubmenu) {
        clearScreen();
        
        // Mostrar cancion actual con estado
        cout << "Actual";
        string estado = getStatusPrefix();
        if (!estado.empty()) {
            cout << " (" << estado << ")";
        }
        if (config.getCurrentSong().isValid()) {
            cout << ": " << config.getCurrentSong().getTitle() << " - " << config.getCurrentSong().getArtist();
        } else {
            cout << ": Vacia";
        }
        cout << "\n\n";
        
        // Mostrar lista de reproduccion
        cout << "Lista de reproduccion actual:\n";
        if (config.getPlaylist().isEmpty()) {
            cout << "Vacia\n\n";
        } else {
            for (int i = 0; i < config.getPlaylist().getSize(); i++) {
                cout << (i + 1) << ". " << config.getPlaylist().get(i).getTitle() 
                     << " - " << config.getPlaylist().get(i).getArtist() << "\n";
            }
            cout << "\n";
        }
        
        // Mostrar opciones
        cout << "Opciones:\n";
        if (!config.getPlaylist().isEmpty()) {
            cout << "S<num> - Saltar a la cancion seleccionada\n";
        }
        cout << "V - Volver al menu principal\n";
        cout << "Ingrese Opcion: ";
        
        getline(cin, option);

        if (option.empty()) {
            continue;
        }
        
        if (option == "V" || option == "v") {
            inSubmenu = false;
        }
        else if ((option[0] == 'S' || option[0] == 's') && !config.getPlaylist().isEmpty()) {
            int num = 0;
            try {
                num = stoi(option.substr(1));
            } catch (...) {
                cout << "Numero invalido\n";
                cout << "Presione Enter para continuar...";
                cin.get();
                continue;
            }
            
            if (num >= 1 && num <= config.getPlaylist().getSize()) {
                // Guardar cancion actual en historial
                if (config.getCurrentSong().isValid()) {
                    addToHistory(config.getCurrentSong());
                }
                
                // Saltar a la cancion seleccionada
                Song selected = config.getPlaylist().get(num - 1);
                
                // Eliminar canciones hasta la seleccionada
                for (int i = 0; i < num; i++) {
                    config.getPlaylist().pop_front();
                }
                
                config.setCurrentSong(selected);
                config.setPlaying(true);
                updateConfigFile();
                inSubmenu = false;  // Volver al menu principal automaticamente
            } else {
                cout << "Numero fuera de rango\n";
                cout << "Presione Enter para continuar...";
                cin.get();
            }
        }
    }
}

void Player::showAllSongsMenu() {
    if (allSongs.isEmpty()) return;
    
    bool inSubmenu = true;
    string option;
    
    while (inSubmenu) {
        clearScreen();
        
        // Mostrar cancion actual
        cout << "Actual";
        string estado = getStatusPrefix();
        if (!estado.empty()) {
            cout << " (" << estado << ")";
        }
        if (config.getCurrentSong().isValid()) {
            cout << ": " << config.getCurrentSong().getTitle() << " - " << config.getCurrentSong().getArtist();
        } else {
            cout << ": Vacia";
        }
        cout << "\n\n";
        
        // Mostrar todas las canciones
        cout << "Canciones registradas:\n";
        for (int i = 0; i < allSongs.getSize(); i++) {
            cout << (i + 1) << ". " << allSongs.get(i).getTitle() 
                 << " - " << allSongs.get(i).getArtist() << "\n";
        }
        
        // Mostrar opciones
        cout << "\nOpciones:\n";
        cout << "R<num> - Reproducir cancion seleccionada\n";
        cout << "A<num> - Agregar cancion seleccionada al final\n";
        cout << "N - Agregar cancion al registro\n";
        cout << "D<num> - Eliminar cancion seleccionada\n";
        cout << "V - Volver al menu principal\n";
        cout << "Ingrese Opcion: ";
        
        getline(cin, option);
        
        if (option == "V" || option == "v") {
            inSubmenu = false;
        }
        else if (option == "N" || option == "n") {
            string title, artist, album, filePath;
            int year, duration;

            auto leerEntero = [](const string& mensaje) {
                while (true) {
                    cout << mensaje;
                    string texto;
                    getline(cin, texto);
                    try {
                        size_t pos = 0;
                        int valor = stoi(texto, &pos);
                        if (pos == texto.size()) return valor;
                    } catch (...) {
                    }
                    cout << "Numero invalido\n";
                }
            };
            
            cout << "Nombre de la cancion: ";
            getline(cin, title);
            cout << "Nombre del artista: ";
            getline(cin, artist);
            cout << "Nombre del album: ";
            getline(cin, album);
            year = leerEntero("Ano de lanzamiento: ");
            duration = leerEntero("Duracion en segundos: ");
            cout << "Ruta del archivo: ";
            getline(cin, filePath);
            
            Song newSong(config.getNextId(), title, artist, album, year, duration, filePath);
            allSongs.push_back(newSong);
            fileManager.addSong("music_source.txt", newSong);
            config.setNextId(config.getNextId() + 1);
            updateConfigFile();
            
            cout << "\nCancion agregada correctamente!\n";
            cout << "Presione Enter para continuar...";
            cin.get();
        }
        else if (!option.empty() && (option[0] == 'R' || option[0] == 'r')) {
            int num = 0;
            try {
                num = stoi(option.substr(1));
            } catch (...) {
                cout << "Numero invalido\n";
                cout << "Presione Enter para continuar...";
                cin.get();
                continue;
            }
            
            if (num >= 1 && num <= allSongs.getSize()) {
                Song selected = allSongs.get(num - 1);
                
                // Vaciar playlist y crear nueva aleatoria
                LinkedList<Song> newPlaylist;
                for (int i = 0; i < allSongs.getSize(); i++) {
                    if (allSongs.get(i).getId() != selected.getId()) {
                        newPlaylist.push_back(allSongs.get(i));
                    }
                }
                newPlaylist.shuffle();
                
                config.setPlaylist(newPlaylist);
                config.setCurrentSong(selected);
                config.setPlaying(true);
                updateConfigFile();
                inSubmenu = false;  // Volver al menu principal
            } else {
                cout << "Numero fuera de rango\n";
                cout << "Presione Enter para continuar...";
                cin.get();
            }
        }
        else if (!option.empty() && (option[0] == 'A' || option[0] == 'a')) {
            int num = 0;
            try {
                num = stoi(option.substr(1));
            } catch (...) {
                cout << "Numero invalido\n";
                cout << "Presione Enter para continuar...";
                cin.get();
                continue;
            }
            
            if (num >= 1 && num <= allSongs.getSize()) {
                Song selected = allSongs.get(num - 1);
                config.getPlaylist().push_back(selected);
                updateConfigFile();
                cout << "\nCancion agregada a la lista de reproduccion!\n";
                cout << "Presione Enter para continuar...";
                cin.get();
            } else {
                cout << "Numero fuera de rango\n";
                cout << "Presione Enter para continuar...";
                cin.get();
            }
        }
        else if (!option.empty() && (option[0] == 'D' || option[0] == 'd')) {
            int num = 0;
            try {
                num = stoi(option.substr(1));
            } catch (...) {
                cout << "Numero invalido\n";
                cout << "Presione Enter para continuar...";
                cin.get();
                continue;
            }
            
            if (num >= 1 && num <= allSongs.getSize()) {
                int songId = allSongs.get(num - 1).getId();
                allSongs.remove(num - 1);
                fileManager.removeSong("music_source.txt", songId);

                if (config.getCurrentSong().isValid() && config.getCurrentSong().getId() == songId) {
                    config.setCurrentSong(Song());
                    config.setPlaying(false);
                }

                LinkedList<Song> filteredPlaylist;
                for (int i = 0; i < config.getPlaylist().getSize(); i++) {
                    if (config.getPlaylist().get(i).getId() != songId) {
                        filteredPlaylist.push_back(config.getPlaylist().get(i));
                    }
                }
                config.setPlaylist(filteredPlaylist);

                LinkedList<Song> filteredHistory;
                for (int i = 0; i < history.getSize(); i++) {
                    if (history.get(i).getId() != songId) {
                        filteredHistory.push_back(history.get(i));
                    }
                }
                history = filteredHistory;

                updateConfigFile();
                cout << "\nCancion eliminada correctamente!\n";
                cout << "Presione Enter para continuar...";
                cin.get();
            } else {
                cout << "Numero fuera de rango\n";
                cout << "Presione Enter para continuar...";
                cin.get();
            }
        }
    }
}

void Player::exit() {
    updateConfigFile();
    cout << "Saliendo del reproductor...\n";
}

void Player::updateConfigFile() {
    config.saveToFile("status.cfg");
}

void Player::regeneratePlaylist() {
    LinkedList<Song> newPlaylist;
    for (int i = 0; i < allSongs.getSize(); i++) {
        newPlaylist.push_back(allSongs.get(i));
    }
    if (config.getShuffleMode()) {
        newPlaylist.shuffle();
    }
    config.setPlaylist(newPlaylist);
}

void Player::addToHistory(const Song& song) {
    history.push_front(song);
    while (history.getSize() > 50) {
        history.pop_back();
    }
}

Song Player::getPreviousFromHistory() {
    if (history.isEmpty()) return Song();
    Song previous = history.get(0);
    history.pop_front();
    return previous;
}

void Player::clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

string Player::getRepeatModeString() const {
    switch (config.getRepeatMode()) {
        case RepeatMode::ONE: return "R1";
        case RepeatMode::ALL: return "RA";
        default: return "";
    }
}

string Player::getStatusPrefix() const {
    string prefix;

    if (config.getShuffleMode()) {
        prefix = "S";
    }

    string repeatStr = getRepeatModeString();
    if (!repeatStr.empty()) {
        if (!prefix.empty()) {
            prefix += "-";
        }
        prefix += repeatStr;
    }

    return prefix;
}

Player::~Player() {}