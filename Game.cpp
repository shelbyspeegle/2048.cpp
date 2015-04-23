#import "Game.h"

// TODO: Let user continue playing after win.

Game::Game() {
    playGrid = Grid();
    this->setup();
}

void Game::setup() {
    loadScoreData();
}

void Game::start() {
    srand((unsigned) time(0)); // Seed rand with this so it is more random

    this->newGame();

    while (true) {
        int uInput = getch();

        if ( interface.inputIsDirectional(uInput)) {
            this->playGrid.shift( interface.uInputMap[uInput] );
        } else if (uInput == 'q') {
            this->finish();
            return;
        } else if (uInput == 'r') {
            this->newGame();
            continue;
        } else {
            continue;
        }

        this->interface.printBoard(this->highScore);

        if ( this->gameOver() || this->reached2048() ) {
            this->interface.printEndMessage( this->gameOver() );

            while (true) {
                uInput = getch();

                if (uInput == 'r') {
                    this->newGame();
                    break;
                } else if (uInput == 'q') {
                    this->finish();
                    return;
                }
            }
        }
    }
}

void Game::loadScoreData() {
    f = fopen(".scores", "r");

    if (f) {
        char line[256];
        if (fgets(line, sizeof(line), f)) {
            highScore = atoi(line);
            fclose(f);
            f = NULL;
        }
    }
}

void Game::newGame() {
    playGrid.clear();

    playGrid.initializeFreeTile(2);
    playGrid.initializeFreeTile(2);

    interface.printBoard(highScore);
}

bool Game::gameOver() {
    return playGrid.isFull() && !playGrid.tilePairsExist();
}

bool Game::reached2048() {
    return playGrid.gridContains2048Tile();
}

void Game::finish() {
    highScore = playGrid.getScore() > highScore ? playGrid.getScore() : highScore;

    f = fopen(".scores", "w");
    fprintf(f, "%i", highScore);
    fclose(f);

    interface.close();
}