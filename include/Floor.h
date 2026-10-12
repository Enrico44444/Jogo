#ifndef FLOOR_H
#define FLOOR_H

#include "Tile.h"

class Floor : public Tile
{
public:
	Floor(Position pos) : Tile(pos, true) {}
	char symbol() const override { return '.'; }   // Desenhado como ponto
};

#endif
