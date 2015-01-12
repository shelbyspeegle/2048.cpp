CC=g++
CFLAGS=-std=c++0x -Wall -g -O
LFLAGS=-lncurses

all: 2048

Tile.o: Tile.cpp Tile.h
	$(CC) $(CFLAGS) -c Tile.cpp

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp

2048: main.o Tile.o
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)

clean:
	/bin/rm -fr 2048 *.o
