/*
 *  2048.cpp
 *
 *  Created on: December 26, 2014
 *      Author: shelbyspeegle
 */


#include <iostream>
#include <ncurses.h>
using namespace std;

#define NUM_TILES 16

// TODO: Handle win case.
// TODO: See if terminal can handle colors.
// TODO: See if terminal can handle CHANGING colors.
// TODO: Change colors to match web version.

const int START_LINE = 3;
int score;
int highScore = 0;
FILE *f;

bool gameOver();
void loadScoreData();
void newGame();
void printBoard();
void printGameOverMessage();
void setup();
void finish();

class Tile {
  public:
    Tile(){
      this->setValue( 0 );
    }

    Tile( int value ){
      this->setValue( value );
    }

    int getValue() {
      return this->value;
    }

    void setValue( int value ) {
      this->value = value;
    }

    Tile copy() {
      Tile t;
      t.setValue( this->getValue() );
      return t;
    }

    string toString() {
      string returnString = "    ";

      if (this->value != 0) {
        string str = to_string(this->value);

        switch (str.length()) {
          case 4:
            returnString[0] = str[0];
            returnString[1] = str[1];
            returnString[2] = str[2];
            returnString[3] = str[3];
            break;
          case 3:
            returnString[3] = str[2];
          case 2:
            returnString[2] = str[1];
          case 1:
            returnString[1] = str[0];
            break;
          default:
            break;
        }
      }

      return returnString;
    }

  private:
    int value;
};

class Grid {
  public:
    Grid(){
      clear();
    };

    void initialize() {
      for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
          Tile t = Tile(0);
          board[i][j] = t;
        }
      }
    }

    void clear() {
      for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
          this->tileAt(i, j).setValue(0);
        }
      }
      numOccupiedTiles = 0;
    }

    Tile tileAt(int col, int row) {
      return this->board[col][row];
    }

    void rotateCCW() {
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

    void shift( int direction ) {
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

    bool shiftRight() {
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

    bool checkNeighborsForMatch( int col, int row ){
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

    bool isFull() {
      return numOccupiedTiles == NUM_TILES;
    }

    int tilePairsExist(){
      for ( int y = 0; y < 4; y++ ) {
        for ( int x = 0; x < 4; x++) {
          if (this->checkNeighborsForMatch( x, y )) {
            return true;
          }
        }
      }
      return 0;
    }

    void initializeFreeTile( int value ){
      Tile * t = randomFreeTile();

      if (t) {
        t->setValue(value);
      }
    }

  private:
    Tile board[4][4];
    int numOccupiedTiles;

    Tile * randomFreeTile(){
      int searchCount = 0;
      while ( true ) {
        int randomRow = rand() % 4;
        int randomCol = rand() % 4;

        if ( this->board[randomRow][randomCol].getValue() == 0 ) {
          return &board[randomRow][randomCol];
        }

        if (searchCount++ == NUM_TILES) {
          return nullptr;
        }
      }
    }
};

Grid playGrid;

int main(int argc, const char * argv[]) {
  srand( (unsigned) time(0) ); // Seed rand with this so it is more random

  setup();
  newGame();

  bool playing = true;

  while( playing ) {
    int uInput = getch();

    switch (uInput) {
      case 'q':
        playing = false;
        break;
      case 'r':
        newGame();
        break;
      case KEY_RIGHT:
        playGrid.shift(0);
        break;
      case KEY_UP:
        playGrid.shift(1);
        break;
      case KEY_LEFT:
        playGrid.shift(2);
        break;
      case KEY_DOWN:
        playGrid.shift(3);
        break;
      default:
        break;
    }

    printBoard();

    if ( gameOver() ) {
      printGameOverMessage();

      while ( true ) {
        uInput = getch();

        if (uInput == 'r') {
          newGame();
          break;
        } else if (uInput == 'q') {
          playing = false;
          break;
        } else {
          ; // Invalid user input. Ignore.
        }
      }
    }
  }

  finish();

  exit( EXIT_SUCCESS );
}

void loadScoreData() {
  f = fopen(".scores", "r");

  if (f) {
    char line[256];
    if ( fgets(line, sizeof(line), f) ) {
      highScore = atoi(line);
      fclose(f);
      f = NULL;
    }
  }
}

void printGameOverMessage() {
  int boardStartX = getmaxx(stdscr)/2 - 12;

  move( START_LINE + 3, boardStartX );
  addch(ACS_ULCORNER);

  for (int i = 0; i < 22; i++) {
    addch(ACS_HLINE);
  }
  addch(ACS_URCORNER);

  move( START_LINE + 4, boardStartX );
  addch(ACS_VLINE);
  mvprintw(START_LINE + 4, boardStartX + 1, "      Game over!      ");
  addch(ACS_VLINE);

  move( START_LINE + 5, boardStartX );
  addch(ACS_VLINE);
  mvprintw(START_LINE + 5, boardStartX + 1, "                      ");
  addch(ACS_VLINE);

  move( START_LINE + 6, boardStartX );
  addch(ACS_VLINE);
  mvprintw(START_LINE + 6, boardStartX + 1, " Press r for new game ");
  addch(ACS_VLINE);

  move( START_LINE + 7, boardStartX );
  addch(ACS_VLINE);
  mvprintw(START_LINE + 7, boardStartX + 1, " or q to quit.        ");
  addch(ACS_VLINE);

  move( START_LINE + 8, boardStartX );
  addch(ACS_LLCORNER);
  for (int i = 0; i < 22; i++) {
    addch(ACS_HLINE);
  }
  addch(ACS_LRCORNER);

  refresh();
}

void newGame() {
  score = 0;

  playGrid.clear();

  playGrid.initializeFreeTile(2);
  playGrid.initializeFreeTile(2);

  printBoard();
}

void printBoard() {
  clear();

  int boardStartX = getmaxx(stdscr)/2 - 12;

  mvprintw( 0, boardStartX + 10, "Score: %6i", score);
  mvprintw( 1, boardStartX + 1, "2048      Best: %6i", highScore);

  for (int i = 0; i < NUM_TILES; i++) {
    int row = i/4;
    int col = i%4;
    int squareStartY = START_LINE + row*3;
    int squareStartX = boardStartX + col*6;
    Tile currentTile = playGrid.tileAt(col, row);

    move(squareStartY, squareStartX);
    addch(ACS_ULCORNER);
    addch(ACS_HLINE);
    addch(ACS_HLINE);
    addch(ACS_HLINE);
    addch(ACS_HLINE);
    addch(ACS_URCORNER);
    mvaddch(squareStartY +1, squareStartX, ACS_VLINE);

    mvprintw(squareStartY +1, squareStartX +1, "%s", currentTile.toString().c_str());
    mvaddch(squareStartY +1, squareStartX +5, ACS_VLINE);
    move(squareStartY +2, squareStartX);
    addch(ACS_LLCORNER);
    addch(ACS_HLINE);
    addch(ACS_HLINE);
    addch(ACS_HLINE);
    addch(ACS_HLINE);
    addch(ACS_LRCORNER);
  }

  refresh();
}

void setup() {
  loadScoreData();
  initscr(); // Start ncurses mode
  noecho(); // Silence user input
  curs_set(0); // Hide the cursor
  keypad( stdscr, TRUE ); // Converts arrow key input to usable chars

  playGrid.initialize();
}

bool gameOver() {
  return playGrid.isFull() && !playGrid.tilePairsExist();
}

void finish() {
  highScore = score > highScore ? score : highScore;

  f = fopen(".scores", "w");
  fprintf(f, "%i", highScore);
  fclose(f);

  endwin(); // End ncurses mode
}
