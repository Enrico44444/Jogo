#ifndef TILE_H
#define TILE_H


#include "Position.h"

class Tile
{
private:
	Position pos;
	bool walkable;

public:
	bool isWalkable();

};

#endif
