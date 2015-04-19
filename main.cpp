/*
 *  2048.cpp
 *
 *  Created on: December 26, 2014
 *      Author: shelbyspeegle
 */


#include <iostream>
#include "Game.h"

using namespace std;

int main(int argc, const char * argv[]) {
    Game game = Game();
    game.start();

    exit( EXIT_SUCCESS );
}