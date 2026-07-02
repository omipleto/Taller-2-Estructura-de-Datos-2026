#include "Player.h"
#include <iostream>
#include <limits>
#include <cstdlib>

using namespace std;

Player::Player() {}

bool Player::initialize() {
    fileManager.loadSongs(allSongs, "music_source.txt");
    ranking.loadFromFile("song_ranking.txt");

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
            cout << ": "
                 << config.getCurrentSong().getTitle()
                 << " - "
                 << config.getCurrentSong().getArtist();
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
        cout << "T - TOP 10 Artistas y Canciones\n";
        cout << "X - Salir\n";
        cout << "Ingrese Opcion: ";

        cin >> option;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (option) {
            case 'W':
            case 'w':
                playPause();
                break;

            case 'Q':
            case 'q':
                previousTrack();
                break;

            case 'E':
            case 'e':
                nextTrack();
                break;

            case 'S':
            case 's':
                toggleShuffle();
                break;

            case 'R':
            case 'r':
                toggleRepeat();
                break;

            case 'A':
            case 'a':
                showCurrentPlaylist();
                break;

            case 'L':
            case 'l':
                showAllSongsMenu();
                break;

            case 'T':
            case 't':
                showTopMenu();
                break;

            case 'X':
            case 'x':
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
    if (allSongs.isEmpty()) {
        return;
    }

    bool wasPlaying = config.isPlaying();

    config.setPlaying(!wasPlaying);

    if (!wasPlaying && config.getCurrentSong().isValid()) {
        registerPlay(config.getCurrentSong());
    }

    updateConfigFile();
}

void Player::previousTrack() {
    if (allSongs.isEmpty()) {
        return;
    }

    Song previous = getPreviousFromHistory();

    if (previous.isValid()) {
        if (config.getCurrentSong().isValid()) {
            addToHistory(config.getCurrentSong());
        }

        config.setCurrentSong(previous);
        config.setPlaying(true);

        registerPlay(previous);
    }

    updateConfigFile();
}

void Player::nextTrack() {
    if (allSongs.isEmpty()) {
        return;
    }

    if (config.getCurrentSong().isValid()) {
        addToHistory(config.getCurrentSong());
    }

    bool songChanged = false;

    if (!config.getPlaylist().isEmpty()) {
        Song nextSong = config.getPlaylist().get(0);

        config.getPlaylist().pop_front();
        config.setCurrentSong(nextSong);
        config.setPlaying(true);

        songChanged = true;
    }
    else if (!allSongs.isEmpty()) {
        if (config.getRepeatMode() == RepeatMode::ALL) {
            regeneratePlaylist();

            if (!config.getPlaylist().isEmpty()) {
                Song nextSong = config.getPlaylist().get(0);

                config.getPlaylist().pop_front();
                config.setCurrentSong(nextSong);
                config.setPlaying(true);

                songChanged = true;
            }
        }
        else if (config.getRepeatMode() == RepeatMode::ONE) {
            config.setPlaying(true);
        }
        else {
            regeneratePlaylist();

            if (!config.getPlaylist().isEmpty()) {
                Song nextSong = config.getPlaylist().get(0);

                config.getPlaylist().pop_front();
                config.setCurrentSong(nextSong);
                config.setPlaying(true);

                songChanged = true;
            }
        }
    }

    if ((songChanged || config.getRepeatMode() == RepeatMode::ONE)
        && config.getCurrentSong().isValid()) {
        registerPlay(config.getCurrentSong());
    }

    updateConfigFile();
}

void Player::toggleShuffle() {
    if (allSongs.isEmpty()) {
        return;
    }

    config.setShuffleMode(!config.getShuffleMode());

    if (config.getShuffleMode()) {
        config.getPlaylist().shuffle();
    }

    updateConfigFile();
}

void Player::toggleRepeat() {
    if (allSongs.isEmpty()) {
        return;
    }

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
    if (allSongs.isEmpty()) {
        return;
    }

    bool inSubmenu = true;
    string option;

    while (inSubmenu) {
        clearScreen();

        cout << "Actual";

        string estado = getStatusPrefix();

        if (!estado.empty()) {
            cout << " (" << estado << ")";
        }

        if (config.getCurrentSong().isValid()) {
            cout << ": "
                 << config.getCurrentSong().getTitle()
                 << " - "
                 << config.getCurrentSong().getArtist();
        } else {
            cout << ": Vacia";
        }

        cout << "\n\n";

        cout << "Lista de reproduccion actual:\n";

        if (config.getPlaylist().isEmpty()) {
            cout << "Vacia\n\n";
        } else {
            for (int i = 0; i < config.getPlaylist().getSize(); i++) {
                cout << (i + 1)
                     << ". "
                     << config.getPlaylist().get(i).getTitle()
                     << " - "
                     << config.getPlaylist().get(i).getArtist()
                     << "\n";
            }

            cout << "\n";
        }

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
        else if ((option[0] == 'S' || option[0] == 's')
                 && !config.getPlaylist().isEmpty()) {
            int num = 0;

            try {
                num = stoi(option.substr(1));
            }
            catch (...) {
                cout << "Numero invalido\n";
                cout << "Presione Enter para continuar...";
                cin.get();
                continue;
            }

            if (num >= 1 && num <= config.getPlaylist().getSize()) {
                if (config.getCurrentSong().isValid()) {
                    addToHistory(config.getCurrentSong());
                }

                Song selected = config.getPlaylist().get(num - 1);

                for (int i = 0; i < num; i++) {
                    config.getPlaylist().pop_front();
                }

                config.setCurrentSong(selected);
                config.setPlaying(true);

                registerPlay(selected);
                updateConfigFile();

                inSubmenu = false;
            }
            else {
                cout << "Numero fuera de rango\n";
                cout << "Presione Enter para continuar...";
                cin.get();
            }
        }
    }
}

void Player::showAllSongsMenu() {
    if (allSongs.isEmpty()) {
        return;
    }

    bool inSubmenu = true;
    string option;

    while (inSubmenu) {
        clearScreen();

        cout << "Actual";

        string estado = getStatusPrefix();

        if (!estado.empty()) {
            cout << " (" << estado << ")";
        }

        if (config.getCurrentSong().isValid()) {
            cout << ": "
                 << config.getCurrentSong().getTitle()
                 << " - "
                 << config.getCurrentSong().getArtist();
        } else {
            cout << ": Vacia";
        }

        cout << "\n\n";

        cout << "Canciones registradas:\n";

        for (int i = 0; i < allSongs.getSize(); i++) {
            cout << (i + 1)
                 << ". "
                 << allSongs.get(i).getTitle()
                 << " - "
                 << allSongs.get(i).getArtist()
                 << "\n";
        }

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
            string title;
            string artist;
            string album;
            string filePath;

            int year;
            int duration;

            auto leerEntero = [](const string& mensaje) {
                while (true) {
                    cout << mensaje;

                    string texto;
                    getline(cin, texto);

                    try {
                        size_t pos = 0;
                        int valor = stoi(texto, &pos);

                        if (pos == texto.size()) {
                            return valor;
                        }
                    }
                    catch (...) {
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

            Song newSong(
                config.getNextId(),
                title,
                artist,
                album,
                year,
                duration,
                filePath
            );

            allSongs.push_back(newSong);
            fileManager.addSong("music_source.txt", newSong);

            config.setNextId(config.getNextId() + 1);

            updateConfigFile();

            cout << "\nCancion agregada correctamente!\n";
            cout << "Presione Enter para continuar...";
            cin.get();
        }
        else if (!option.empty()
                 && (option[0] == 'R' || option[0] == 'r')) {
            int num = 0;

            try {
                num = stoi(option.substr(1));
            }
            catch (...) {
                cout << "Numero invalido\n";
                cout << "Presione Enter para continuar...";
                cin.get();
                continue;
            }

            if (num >= 1 && num <= allSongs.getSize()) {
                Song selected = allSongs.get(num - 1);

                playSelectedSong(selected);

                inSubmenu = false;
            }
            else {
                cout << "Numero fuera de rango\n";
                cout << "Presione Enter para continuar...";
                cin.get();
            }
        }
        else if (!option.empty()
                 && (option[0] == 'A' || option[0] == 'a')) {
            int num = 0;

            try {
                num = stoi(option.substr(1));
            }
            catch (...) {
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
            }
            else {
                cout << "Numero fuera de rango\n";
                cout << "Presione Enter para continuar...";
                cin.get();
            }
        }
        else if (!option.empty()
                 && (option[0] == 'D' || option[0] == 'd')) {
            int num = 0;

            try {
                num = stoi(option.substr(1));
            }
            catch (...) {
                cout << "Numero invalido\n";
                cout << "Presione Enter para continuar...";
                cin.get();
                continue;
            }

            if (num >= 1 && num <= allSongs.getSize()) {
                int songId = allSongs.get(num - 1).getId();

                allSongs.remove(num - 1);
                fileManager.removeSong("music_source.txt", songId);

                ranking.removeSong(songId);
                updateRankingFile();

                if (config.getCurrentSong().isValid()
                    && config.getCurrentSong().getId() == songId) {
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
            }
            else {
                cout << "Numero fuera de rango\n";
                cout << "Presione Enter para continuar...";
                cin.get();
            }
        }
    }
}

void Player::showTopMenu() {
    bool inSubmenu = true;
    string option;

    while (inSubmenu) {
        clearScreen();

        cout << "Ranking TOP\n\n";
        cout << "C - Top 10 canciones mas escuchadas\n";
        cout << "A - Top 10 artistas mas escuchados\n";
        cout << "X - Salir\n";
        cout << "Ingrese Opcion: ";

        getline(cin, option);

        if (option == "C" || option == "c") {
            showTopSongs();
        }
        else if (option == "A" || option == "a") {
            showTopArtists();
        }
        else if (option == "X" || option == "x") {
            inSubmenu = false;
        }
    }
}

void Player::showTopSongs() {
    bool inSubmenu = true;

    while (inSubmenu) {
        MaxHeap heap = ranking.buildSongHeap(allSongs);

        int amount = heap.getSize();

        if (amount > 10) {
            amount = 10;
        }

        HeapItem* topSongs = amount > 0 ? new HeapItem[amount] : nullptr;

        for (int i = 0; i < amount; i++) {
            topSongs[i] = heap.pop();
        }

        clearScreen();

        cout << "Ranking TOP "
             << amount
             << " Canciones mas escuchadas:\n\n";

        if (amount == 0) {
            cout << "No hay canciones registradas.\n";
        }
        else {
            for (int i = 0; i < amount; i++) {
                cout << (i + 1)
                     << ". ["
                     << topSongs[i].reproductions
                     << "] "
                     << topSongs[i].primaryText
                     << " - "
                     << topSongs[i].secondaryText
                     << "\n";
            }
        }

        cout << "\nOpciones:\n";

        if (amount > 0) {
            cout << "R<num> - Reproducir cancion seleccionada\n";
            cout << "A<num> - Agregar cancion seleccionada al final de la lista de reproduccion actual\n";
        }

        cout << "A - Top 10 artistas mas escuchados\n";
        cout << "V - Volver al menu principal\n";
        cout << "Ingrese Opcion: ";

        string option;
        getline(cin, option);

        if (option == "V" || option == "v") {
            inSubmenu = false;
        }
        else if (option == "A" || option == "a") {
            delete[] topSongs;
            showTopArtists();
            continue;
        }
        else if (!option.empty()
                 && (option[0] == 'R' || option[0] == 'r')) {
            int num = 0;

            try {
                num = stoi(option.substr(1));
            }
            catch (...) {
                num = 0;
            }

            if (num >= 1 && num <= amount) {
                Song selected = getSongById(topSongs[num - 1].id);

                if (selected.isValid()) {
                    playSelectedSong(selected);
                    inSubmenu = false;
                }
            }
        }
        else if (!option.empty()
                 && (option[0] == 'A' || option[0] == 'a')) {
            int num = 0;

            try {
                num = stoi(option.substr(1));
            }
            catch (...) {
                num = 0;
            }

            if (num >= 1 && num <= amount) {
                Song selected = getSongById(topSongs[num - 1].id);

                if (selected.isValid()) {
                    config.getPlaylist().push_back(selected);

                    updateConfigFile();

                    cout << "\nCancion agregada a la lista de reproduccion!\n";
                    cout << "Presione Enter para continuar...";
                    cin.get();
                }
            }
        }

        delete[] topSongs;
    }
}

void Player::showTopArtists() {
    bool inSubmenu = true;

    while (inSubmenu) {
        MaxHeap heap = ranking.buildArtistHeap(allSongs);

        int amount = heap.getSize();

        if (amount > 10) {
            amount = 10;
        }

        HeapItem* topArtists = amount > 0 ? new HeapItem[amount] : nullptr;

        for (int i = 0; i < amount; i++) {
            topArtists[i] = heap.pop();
        }

        clearScreen();

        cout << "Ranking TOP "
             << amount
             << " Artistas mas escuchados:\n\n";

        if (amount == 0) {
            cout << "No hay artistas registrados.\n";
        }
        else {
            for (int i = 0; i < amount; i++) {
                cout << (i + 1)
                     << ". ["
                     << topArtists[i].reproductions
                     << "] "
                     << topArtists[i].primaryText
                     << "\n";
            }
        }

        cout << "\nOpciones:\n";

        if (amount > 0) {
            cout << "S<num> - Mostrar canciones del artista\n";
        }

        cout << "C - Top 10 canciones mas escuchadas\n";
        cout << "V - Volver al menu principal\n";
        cout << "Ingrese Opcion: ";

        string option;
        getline(cin, option);

        if (option == "V" || option == "v") {
            inSubmenu = false;
        }
        else if (option == "C" || option == "c") {
            delete[] topArtists;
            showTopSongs();
            continue;
        }
        else if (!option.empty()
                 && (option[0] == 'S' || option[0] == 's')) {
            int num = 0;

            try {
                num = stoi(option.substr(1));
            }
            catch (...) {
                num = 0;
            }

            if (num >= 1 && num <= amount) {
                bool returnToMain = showArtistSongs(
                    topArtists[num - 1].primaryText
                );

                if (returnToMain) {
                    inSubmenu = false;
                }
            }
        }

        delete[] topArtists;
    }
}

bool Player::showArtistSongs(const string& artist) {
    int totalSongs = allSongs.getSize();

    Song* artistSongs = totalSongs > 0 ? new Song[totalSongs] : nullptr;
    int artistSongCount = 0;

    for (int i = 0; i < totalSongs; i++) {
        if (allSongs.get(i).getArtist() == artist) {
            artistSongs[artistSongCount] = allSongs.get(i);
            artistSongCount++;
        }
    }

    for (int i = 0; i < artistSongCount - 1; i++) {
        for (int j = 0; j < artistSongCount - i - 1; j++) {
            if (artistSongs[j].getTitle()
                > artistSongs[j + 1].getTitle()) {
                Song temp = artistSongs[j];
                artistSongs[j] = artistSongs[j + 1];
                artistSongs[j + 1] = temp;
            }
        }
    }

    bool inSubmenu = true;
    bool returnToMain = false;

    while (inSubmenu) {
        clearScreen();

        cout << "Ranking TOP 10 Artistas mas escuchados:\n\n";
        cout << "Artista: " << artist << "\n";

        for (int i = 0; i < artistSongCount; i++) {
            cout << (i + 1)
                 << ". "
                 << artistSongs[i].getTitle()
                 << "\n";
        }

        cout << "\nOpciones:\n";

        if (artistSongCount > 0) {
            cout << "R<num> - Reproducir cancion seleccionada\n";
            cout << "A<num> - Agregar cancion seleccionada al final de la lista de reproduccion actual\n";
        }

        cout << "V - Volver al listado de TOP 10 artistas\n";
        cout << "X - Volver al menu principal\n";
        cout << "Ingrese Opcion: ";

        string option;
        getline(cin, option);

        if (option == "V" || option == "v") {
            inSubmenu = false;
        }
        else if (option == "X" || option == "x") {
            inSubmenu = false;
            returnToMain = true;
        }
        else if (!option.empty()
                 && (option[0] == 'R' || option[0] == 'r')) {
            int num = 0;

            try {
                num = stoi(option.substr(1));
            }
            catch (...) {
                num = 0;
            }

            if (num >= 1 && num <= artistSongCount) {
                playSelectedSong(artistSongs[num - 1]);

                inSubmenu = false;
                returnToMain = true;
            }
        }
        else if (!option.empty()
                 && (option[0] == 'A' || option[0] == 'a')) {
            int num = 0;

            try {
                num = stoi(option.substr(1));
            }
            catch (...) {
                num = 0;
            }

            if (num >= 1 && num <= artistSongCount) {
                config.getPlaylist().push_back(artistSongs[num - 1]);

                updateConfigFile();

                cout << "\nCancion agregada a la lista de reproduccion!\n";
                cout << "Presione Enter para continuar...";
                cin.get();
            }
        }
    }

    delete[] artistSongs;

    return returnToMain;
}

void Player::exit() {
    updateConfigFile();
    updateRankingFile();

    cout << "Saliendo del reproductor...\n";
}

void Player::updateConfigFile() {
    config.saveToFile("status.cfg");
}

void Player::updateRankingFile() {
    ranking.saveToFile("song_ranking.txt");
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
    if (history.isEmpty()) {
        return Song();
    }

    Song previous = history.get(0);

    history.pop_front();

    return previous;
}

Song Player::getSongById(int songId) const {
    for (int i = 0; i < allSongs.getSize(); i++) {
        if (allSongs.get(i).getId() == songId) {
            return allSongs.get(i);
        }
    }

    return Song();
}

void Player::registerPlay(const Song& song) {
    if (!song.isValid()) {
        return;
    }

    ranking.registerPlay(song.getId());
    updateRankingFile();
}

void Player::playSelectedSong(const Song& song) {
    if (!song.isValid()) {
        return;
    }

    if (config.getCurrentSong().isValid()
        && config.getCurrentSong().getId() != song.getId()) {
        addToHistory(config.getCurrentSong());
    }

    LinkedList<Song> newPlaylist;

    for (int i = 0; i < allSongs.getSize(); i++) {
        if (allSongs.get(i).getId() != song.getId()) {
            newPlaylist.push_back(allSongs.get(i));
        }
    }

    newPlaylist.shuffle();

    config.setPlaylist(newPlaylist);
    config.setCurrentSong(song);
    config.setPlaying(true);

    registerPlay(song);
    updateConfigFile();
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
        case RepeatMode::ONE:
            return "R1";

        case RepeatMode::ALL:
            return "RA";

        default:
            return "";
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

void Player::rebuildTrie() {
    songTrie.clear();
    for(int i = 0; i<allSongs.getSize(); i++){
        const Song& song = allSongs.get(i);
        // insertar por titulo
        songTrie.insert(song.getTitle(), song.getId());

        // insertar por artista
        songTrie.insert(song.getArtist(), song.getId());
    }
    
}

void Player::searchSongs() {
    if (allSongs.isEmpty()) {
        cout << "\nNo hay canciones registradas.\n";
        cout << "Presione Enter para continuar...";
        cin.get();
        return;
    }

    string searchText;
    cout << "\nBúsqueda de canciones\n";
    cout << "Buscar canciones que contengan: ";
    getline(cin, searchText);

    if (searchText.empty()) {
        return;
    }

    // Buscar en el Trie
    std::vector<int> results = songTrie.search(searchText);

    if (results.empty()) {
        cout << "\nNo se encontraron canciones que contengan \"" << searchText << "\"\n";
        cout << "Presione Enter para continuar...";
        cin.get();
        return;
    }

    // Mostrar resultados en un submenú
    bool inSubmenu = true;
    string option;

    while (inSubmenu) {
        clearScreen();

        cout << "Búsqueda de canciones\n";
        cout << "Canciones que contienen \"" << searchText << "\":\n\n";

        // Mostrar canciones encontradas
        for (size_t i = 0; i < results.size(); i++) {
            Song song = getSongById(results[i]);
            if (song.isValid()) {
                cout << (i + 1) << ". " << song.getTitle() << " - " << song.getArtist() << "\n";
            }
        }

        cout << "\nOpciones: ";
        cout << "R<num> - Reproducir canción seleccionada ";
        cout << "A<num> - Agregar canción seleccionada al final ";
        cout << "F - Repetir búsqueda con un texto diferente ";
        cout << "V - Volver al menú principal\n";
        cout << "Ingrese Opción: ";

        getline(cin, option);

        if (option == "V" || option == "v") {
            inSubmenu = false;
        }
        else if (option == "F" || option == "f") {
            // Repetir búsqueda con otro texto
            inSubmenu = false;
            searchSongs(); // Llamada recursiva para nueva búsqueda
            return;
        }
        else if (!option.empty() && (option[0] == 'R' || option[0] == 'r')) {
            int num = 0;
            try {
                num = std::stoi(option.substr(1));
            } catch (...) {
                cout << "Número inválido\n";
                cout << "Presione Enter para continuar...";
                cin.get();
                continue;
            }

            if (num >= 1 && num <= (int)results.size()) {
                Song selected = getSongById(results[num - 1]);
                if (selected.isValid()) {
                    playSelectedSong(selected);
                    inSubmenu = false;
                }
            } else {
                cout << "Número fuera de rango\n";
                cout << "Presione Enter para continuar...";
                cin.get();
            }
        }
        else if (!option.empty() && (option[0] == 'A' || option[0] == 'a')) {
            int num = 0;
            try {
                num = std::stoi(option.substr(1));
            } catch (...) {
                cout << "Número inválido\n";
                cout << "Presione Enter para continuar...";
                cin.get();
                continue;
            }

            if (num >= 1 && num <= (int)results.size()) {
                Song selected = getSongById(results[num - 1]);
                if (selected.isValid()) {
                    config.getPlaylist().push_back(selected);
                    updateConfigFile();
                    cout << "\nCanción agregada a la lista de reproducción!\n";
                    cout << "Presione Enter para continuar...";
                    cin.get();
                }
            } else {
                cout << "Número fuera de rango\n";
                cout << "Presione Enter para continuar...";
                cin.get();
            }
        }
        else {
            cout << "Opción inválida\n";
            cout << "Presione Enter para continuar...";
            cin.get();
        }
    }
}

Player::~Player() {}