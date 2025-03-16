#include "inc/Game.hpp"
int main()
{   
    int size=70;
    Game* my_game = new Game(size);
    delete my_game;
    return 0;
}