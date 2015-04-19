#import "Game.h"

// TODO: Handle win case.

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

    bool playing = true;

    while (playing) {
        int uInput = getch();

        if ( interface.inputIsDirectional(uInput)) {
            this->playGrid.shift( interface.uInputMap[uInput] );
        } else if (uInput == 'q') {
            playing = false;
        } else if (uInput == 'r') {
            this->newGame();
        } else {
            continue;
        }

        this->interface.printBoard(this->playGrid.getScore(), this->highScore);

        if (this->gameOver()) {
            this->interface.printGameOverMessage();

            while (true) {
                uInput = getch();

                if (uInput == 'r') {
                    this->newGame();
                    break;
                } else if (uInput == 'q') {
                    playing = false;
                    break;
                } else {; // Invalid user input. Ignore.
                }
            }
        }
    }

    this->finish();
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

    interface.printBoard(playGrid.getScore(), highScore);
}

bool Game::gameOver() {
    return playGrid.isFull() && !playGrid.tilePairsExist();
}

void Game::finish() {
    highScore = playGrid.getScore() > highScore ? playGrid.getScore() : highScore;

    f = fopen(".scores", "w");
    fprintf(f, "%i", highScore);
    fclose(f);

    interface.close();
}