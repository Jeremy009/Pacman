#include "Game.h"
#include <QApplication>


//Define a global variable game, so that we get access
//to the game's attributes at all times.
Game *game;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    game = new Game();

    return a.exec();
}
