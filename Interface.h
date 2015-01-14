#ifndef INTERFACE_H
#define INTERFACE_H

#include "Grid.h"
#include <ncurses.h>

#define START_LINE 3

class Interface {

public:
    const static int RIGHT = KEY_RIGHT;
    const static int UP = KEY_UP;
    const static int LEFT = KEY_LEFT;
    const static int DOWN = KEY_DOWN;

    Interface( Grid *grid );
    void printGameOverMessage();
    void printBoard( int score, int highScore );
    void drawTile( Tile *tile );
    void close();

private:
    int boardStartX;
    Grid *playGrid;
};
#endif