/*
 *  2048.cpp
 *
 *  Created on: December 26, 2014
 *      Author: shelbyspeegle
 */


#include <iostream>
#include <ncurses.h>
#include "Interface.h"
using namespace std;

// TODO: Handle win case.
// TODO: See if terminal can handle colors.
// TODO: See if terminal can handle CHANGING colors.
// TODO: Change colors to match web version.

int highScore = 0;
FILE *f;
Grid playGrid;
Interface interface(&playGrid);

bool gameOver();
void loadScoreData();
void newGame();
void setup();
void finish();

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

    interface.printBoard( playGrid.getScore(), highScore );

    if ( gameOver() ) {
      interface.printGameOverMessage();

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

void newGame() {
  playGrid.clear();

  playGrid.initializeFreeTile(2);
  playGrid.initializeFreeTile(2);

  interface.printBoard( playGrid.getScore(), highScore );
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