#ifndef GAME_H
#define GAME_H

#include <ncurses.h>
#include <iostream>
#include "Interface.h"

class Game {

public:
    Game();
    void start();

private:
    int highScore = 0;
    FILE *f;
    Grid playGrid;
    Interface interface = Interface( &playGrid );

    bool gameOver();
    void loadScoreData();
    void newGame();
    void setup();
    void finish();
};

#endif