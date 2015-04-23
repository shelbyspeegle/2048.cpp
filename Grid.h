#ifndef GRID_H
#define GRID_H

#include "Tile.h"

class Grid {

public:
    Grid();
    void clear();
    Tile tileAt( int col, int row );
    void rotateCCW();
    void shift( int direction );
    bool shiftRight();
    bool checkNeighborsForMatch( int col, int row );
    int getNumOccupiedTiles();
    bool isFull();
    int tilePairsExist();
    void initializeFreeTile( int value );
    int getScore();
    bool gridContains2048Tile();

private:
    int score;
    Tile board[4][4];

    Tile * randomFreeTile();
};

#endif