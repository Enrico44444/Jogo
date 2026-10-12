#include "Tile.h"

// Tile vazio: posição (0, 0) e não andável
Tile::Tile() : pos(), walkable(false) {}

// Tile na posição dada, andável ou não
Tile::Tile(Position pos, bool walkable) : pos(pos), walkable(walkable) {}

// Símbolo padrão; as subclasses devolvem o seu ('.', '#', 'D')
char Tile::symbol() const
{
	return '?';
}
