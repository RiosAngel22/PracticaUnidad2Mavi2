#include "Game.h"

using namespace sf;

// Función principal del programa
int main(int argc, char* argv[])
{
    // Creamos una instancia del juego
    Game* Juego;
    // Inicializamos el juego con una ventana de 800x600 píxeles con el título "Avatar"
    Juego = new Game(800, 600, "Actividad 5");
    // Iniciamos el bucle principal del juego
    Juego->Loop();

    return 0;
}
