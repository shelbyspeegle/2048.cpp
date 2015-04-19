#include <iostream>
#include "Grid.h"

#define NUM_TILES 16

Grid::Grid(){
    clear();

    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            Tile t = Tile(0);
            t.x = i;
            t.y = j;
            board[i][j] = t;
        }
    }
};

void Grid::clear() {
    score = 0;

    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            this->tileAt(i, j).setValue(0);
        }
    }
    numOccupiedTiles = 0;
}

Tile Grid::tileAt(int col, int row) {
    return this->board[col][row];
}

void Grid::rotateCCW() {
    Tile tempBoard[4][4];

    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            tempBoard[i][j] = board[i][j].copy();
        }
    }

    for(int y = 0; y < 4; y++) {
        for(int x = 0; x < 4; x++) {
            board[x][y] = tempBoard[3-y][x];
        }
    }
}

void Grid::shift( int direction ) {
    int numberOfRotationsBefore = 0;
    int numberOfRotationsAfter = direction;

    // Rotations before.
    if (direction != 0) {
        numberOfRotationsBefore = 4-direction;
    }

    for (int i = 0; i < numberOfRotationsBefore; i++) {
        this->rotateCCW();
    }

    bool changeHappened = shiftRight();

    // Rotations after.
    for (int i = 0; i < numberOfRotationsAfter; i++) {
        rotateCCW();
    }

    if (changeHappened) {
        // Set a random free square on the board to a 2 or 4.
        int newSquareValue = 2 * ((rand() % 2 ) + 1);
        this->initializeFreeTile(newSquareValue);
    }
}

bool Grid::shiftRight() {
    int row;
    int tempBoard[4][4];

    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            tempBoard[i][j]= this->tileAt(i, j).getValue();
        }
    }

    for ( row = 0; row < 4; row++ ) {
        int icol = 3;
        int jcol = 2;
        int rowLimit = 0;
        while (icol >= rowLimit && jcol >= rowLimit) {
            if (this->tileAt(icol, row).getValue() == 0) {
                icol--;
                jcol--;
            } else if (this->tileAt(jcol, row).getValue() == 0) {
                jcol--;
            } else if (this->tileAt(icol,row).getValue() == this->tileAt(jcol,row).getValue()) {
                this->board[icol][row].setValue( this->tileAt(icol, row).getValue() * 2 );
                score += this->tileAt(icol, row).getValue();
                this->board[jcol][row].setValue(0);
                icol = jcol - 1;
                jcol = icol - 1;
            } else {
                icol--;
                jcol = icol - 1;
            }
        }

        jcol = 3;

        for (icol = jcol; icol >= rowLimit; icol--) { // 0, 4, 8, 12
            if (this->tileAt(icol, row).getValue() != 0) {
                this->board[jcol][row].setValue( this->tileAt(icol, row).getValue() );
                jcol--;
            }
        }

        for (; jcol >= rowLimit; jcol--) {
            this->board[jcol][row].setValue(0);
        }
    }

    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            if ( tempBoard[x][y] != this->tileAt(x, y).getValue() ) {
                return true; // Change happened.
            }
        }
    }

    return false; // No change happened.
}

bool Grid::checkNeighborsForMatch( int col, int row ){
    // Check East.
    if ( col < 3 ) {
        if ( this->board[col][row].getValue() == this->board[col+1][row].getValue() ) {
            return true;
        }
    }

    // Check North.
    if ( row > 0 ) {
        if ( this->board[col][row].getValue() == this->board[col][row-1].getValue() ) {
            return true;
        }
    }

    // Check West.
    if ( col > 1 ) {
        if ( this->board[col][row].getValue() == this->board[col-1][row].getValue() ) {
            return true;
        }
    }

    // Check South.
    if ( row < 3 ) {
        if ( this->board[col][row].getValue() == this->board[col][row+1].getValue() ) {
            return true;
        }
    }
    return false;
}

bool Grid::isFull() {
    return numOccupiedTiles == NUM_TILES;
}

int Grid::tilePairsExist(){
    for ( int y = 0; y < 4; y++ ) {
        for ( int x = 0; x < 4; x++) {
            if (this->checkNeighborsForMatch( x, y )) {
                return true;
            }
        }
    }
    return 0;
}

void Grid::initializeFreeTile( int value ){
    Tile * t = randomFreeTile();

    if (t) {
        t->setValue(value);
    }
}

Tile * Grid::randomFreeTile(){
    if ( this->isFull() ) {
        return nullptr;
    }

    while ( true ) {
        int randomRow = rand() % 4;
        int randomCol = rand() % 4;

        if ( this->board[randomRow][randomCol].getValue() == 0 ) {
            return &board[randomRow][randomCol];
        }
    }
}

int Grid::getScore() {
    return score;
}