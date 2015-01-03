CC=g++
CFLAGS=-Wall -g -O
LFLAGS=-lncurses
PROGS=2048

all: $(PROGS)

2048: 2048.o
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)

clean:
	/bin/rm -f $(PROGS) *.o a.out *.d
	/bin/rm -fr *.dSYM
