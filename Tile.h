#ifndef TILE_H
#define TILE_H

#include <string>

class Tile {

public:
  int x;
  int y;

  Tile();
  Tile( int value );
  int getValue();
  void setValue( int value );
  Tile copy();
  std::string toString();

private:
  int value;
};

#endif