#ifndef GRID_H
#define GRID_H

#include "Tile.h"

class Grid {

public:
    Grid();
    void initialize();
    void clear();
    Tile tileAt( int col, int row );
    void rotateCCW();
    void shift( int direction );
    bool shiftRight();
    bool checkNeighborsForMatch( int col, int row );
    bool isFull();
    int tilePairsExist();
    void initializeFreeTile( int value );
    int getScore();

private:
    int score;
    Tile board[4][4];
    int numOccupiedTiles;

    Tile * randomFreeTile();
};

#endif