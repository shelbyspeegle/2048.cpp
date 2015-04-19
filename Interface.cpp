#include "Interface.h"

// TODO: See if terminal can handle colors.
// TODO: See if terminal can handle CHANGING colors.
// TODO: Change colors to match web version.

Interface::Interface( Grid *grid ) {
    playGrid = grid;

    initscr(); // Start ncurses mode
    noecho(); // Silence user input
    curs_set(0); // Hide the cursor
    keypad( stdscr, TRUE ); // Converts arrow key input to usable chars.
}

void Interface::printGameOverMessage() {
    int currentLine = START_LINE + 3;

    std::string lines[] = {
            "      Game over!      ",
            "                      ",
            " Press r for new game ",
            " or q to quit.        "
    };

    boardStartX = getmaxx(stdscr)/2 - 12;

    move( currentLine++, boardStartX );
    addch(ACS_ULCORNER);
    for (int i = 0; i < 22; i++) {
        addch(ACS_HLINE);
    }
    addch(ACS_URCORNER);


    for (int i = 0; i < 4; i++) {
        move( currentLine, boardStartX );
        addch(ACS_VLINE);
        mvprintw( currentLine++, boardStartX + 1, lines[i].c_str() );
        addch(ACS_VLINE);
    }

    move( currentLine, boardStartX );
    addch(ACS_LLCORNER);
    for (int i = 0; i < 22; i++) {
        addch(ACS_HLINE);
    }
    addch(ACS_LRCORNER);

    refresh();
}

void Interface::printBoard( int highScore ) {
    clear();

    boardStartX = getmaxx(stdscr)/2 - 12;

    mvprintw( 0, boardStartX + 10, "Score: %6i", playGrid->getScore() );
    mvprintw( 1, boardStartX + 1, "2048      Best: %6i", highScore );

    for (int col = 0; col < 4; col++) {
        for (int row = 0; row < 4; row++) {
            Tile currentTile = playGrid->tileAt(col, row);
            drawTile( &currentTile );
        }
    }

    refresh();
}

void Interface::drawTile( Tile *tile ) {
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

void Interface::close() {
    endwin(); // End ncurses mode
}

bool Interface::inputIsDirectional( int uInput ) {
    return ( uInput == KEY_RIGHT || uInput == KEY_UP || uInput == KEY_LEFT || uInput == KEY_DOWN );
}
