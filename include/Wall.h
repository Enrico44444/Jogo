#ifndef WALL_H
#define WALL_H


#include "Tile.h"

class Wall : public Tile
{
public:
    Wall(Position pos) : Tile(pos, false) {}   // Não andável (false)
    char symbol() const override { return '#'; }   // Desenhada como #
};
#endif
