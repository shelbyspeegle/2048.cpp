#ifndef INTERFACE_H
#define INTERFACE_H

#include "Grid.h"
#include <ncurses.h>

class Interface {

public:
    int boardStartX;
    Grid *playGrid;

    Interface( Grid *grid );

    void initialize( Grid *grid );
    void printGameOverMessage();
    void printBoard( int score, int highScore );
    void drawTile( Tile *tile );
};
#endif