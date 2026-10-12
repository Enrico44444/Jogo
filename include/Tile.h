#ifndef TILE_H
#define TILE_H


#include "Position.h"

// Célula do mapa. É a base de Floor, Wall e Door.
class Tile
{
private:
	Position pos;
	bool walkable;

public:
	Tile();   // Tile vazio, não andável
	Tile(Position pos, bool walkable);   // Tile na posição dada
	virtual ~Tile() = default;   // Virtual: permite apagar uma Door ou Wall por um ponteiro Tile*
	bool isWalkable() const;   // Diz se dá para pisar (ainda não implementada)
	Position getPos() const;   // Devolve a posição do tile (ainda não implementada)
	virtual char symbol() const;   // Caractere usado para desenhar o tile; cada subclasse redefine

};

#endif
