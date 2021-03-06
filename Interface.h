#ifndef INTERFACE_H
#define INTERFACE_H

#include "Grid.h"
#include <ncurses.h>
#include <map>
#include <string.h>

#define START_LINE 3

class Interface {

public:
    std::map<int, int> uInputMap = {
            {KEY_RIGHT, 0},
            {KEY_UP, 1},
            {KEY_LEFT, 2},
            {KEY_DOWN, 3}
    };

    Interface( Grid *grid );
    void printEndMessage( bool gameOver );
    void printBoard( int highScore );
    void drawTile( Tile *tile );
    void close();
    static bool inputIsDirectional( int uInput );

private:
    int boardStartX;
    Grid *playGrid;
};
#endif