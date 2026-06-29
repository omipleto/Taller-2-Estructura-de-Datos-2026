#include "Player.h"
#include <iostream>

int main() {
    Player player;
    
    if (!player.initialize()) {
        std::cout << "Error al inicializar el reproductor" << std::endl;
        return 1;
    }
    
    player.run();
    
    return 0;
}