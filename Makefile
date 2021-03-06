CC=g++
CFLAGS=-std=c++0x -Wall -g -O
LFLAGS=-lncurses

all: 2048

2048: main.o Game.o Interface.o Tile.o Grid.o
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp

Game.o: Game.cpp Game.h
	$(CC) $(CFLAGS) -c Game.cpp

Interface.o: Interface.cpp Interface.h
	$(CC) $(CFLAGS) -c Interface.cpp

Tile.o: Tile.cpp Tile.h
	$(CC) $(CFLAGS) -c Tile.cpp

Grid.o: Grid.cpp Grid.h
	$(CC) $(CFLAGS) -c Grid.cpp

clean:
	/bin/rm -fr 2048 *.o
