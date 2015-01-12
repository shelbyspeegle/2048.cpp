/*
 *  2048.cpp
 *
 *  Created on: December 26, 2014
 *      Author: shelbyspeegle
 */


#include <iostream>
#include <ncurses.h>
#include "Grid.h"
using namespace std;

// TODO: Handle win case.
// TODO: See if terminal can handle colors.
// TODO: See if terminal can handle CHANGING colors.
// TODO: Change colors to match web version.

const int START_LINE = 3;
int highScore = 0;
int boardStartX;
FILE *f;

bool gameOver();
void loadScoreData();
void newGame();
void printBoard();
void printGameOverMessage();
void setup();
void finish();
void drawTile( Tile *tile );

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
  boardStartX = getmaxx(stdscr)/2 - 12;

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
  playGrid.clear();

  playGrid.initializeFreeTile(2);
  playGrid.initializeFreeTile(2);

  printBoard();
}

void printBoard() {
  clear();

  boardStartX = getmaxx(stdscr)/2 - 12;

  mvprintw( 0, boardStartX + 10, "Score: %6i", playGrid.getScore() );
  mvprintw( 1, boardStartX + 1, "2048      Best: %6i", highScore);

  for (int col = 0; col < 4; col++) {
    for (int row = 0; row < 4; row++) {
      Tile currentTile = playGrid.tileAt(col, row);
      drawTile( &currentTile );
    }
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
  highScore = playGrid.getScore() > highScore ? playGrid.getScore() : highScore;

  f = fopen(".scores", "w");
  fprintf(f, "%i", highScore);
  fclose(f);

  endwin(); // End ncurses mode
}

void drawTile( Tile *tile ) {

  int squareStartY = START_LINE + tile->y*3;
  int squareStartX = boardStartX + tile->x*6;

  move(squareStartY, squareStartX);
  addch(ACS_ULCORNER);
  addch(ACS_HLINE);
  addch(ACS_HLINE);
  addch(ACS_HLINE);
  addch(ACS_HLINE);
  addch(ACS_URCORNER);
  mvaddch(squareStartY +1, squareStartX, ACS_VLINE);

  mvprintw(squareStartY +1, squareStartX +1, "%s", tile->toString().c_str());
  mvaddch(squareStartY +1, squareStartX +5, ACS_VLINE);
  move(squareStartY +2, squareStartX);
  addch(ACS_LLCORNER);
  addch(ACS_HLINE);
  addch(ACS_HLINE);
  addch(ACS_HLINE);
  addch(ACS_HLINE);
  addch(ACS_LRCORNER);

}