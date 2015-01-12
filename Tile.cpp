#include <iostream>
#include "Tile.h"
using namespace std;

Tile::Tile(){
  this->setValue( 0 );
}

Tile::Tile( int value ){
  this->setValue( value );
}

int Tile::getValue() {
  return this->value;
}

Tile Tile::copy() {
  Tile t;
  t.setValue( this->getValue() );
  t.x = this->x;
  t.y = this->y;
  return t;
}

void Tile::setValue( int value ) {
  this->value = value;
}

string Tile::toString() {
  string returnString = "    ";

  if (this->value != 0) {
    string str = to_string(this->value);

    switch (str.length()) {
      case 4:
        returnString[0] = str[0];
        returnString[1] = str[1];
        returnString[2] = str[2];
        returnString[3] = str[3];
        break;
      case 3:
        returnString[3] = str[2];
      case 2:
        returnString[2] = str[1];
      case 1:
        returnString[1] = str[0];
        break;
      default:
        break;
    }
  }

  return returnString;
}