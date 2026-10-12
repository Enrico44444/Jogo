#include "Position.h"

// Construtor padrão: posição (0, 0)
Position::Position() : x(0), y(0) {}

// Construtor com a coluna e a linha
Position::Position(int x, int y) : x(x), y(y){}

// Devolve a coluna
int Position::getX() const
{
	return x;
}

// Devolve a linha
int Position::getY() const
{
	return y;
}

// Soma o deslocamento (dx, dy) à posição atual
void Position::move(int dx, int dy)
{
	x += dx;
	y += dy;
}

// Devolve a posição vizinha na direção dada sem alterar a posição atual. Uma direção inválida devolve a mesma posição.
// O y cresce para baixo, como as linhas do mapa.
Position Position::nextPos(char direction) const
{
	switch (direction)
	{
		case 'w': case 'W': return Position(x,y - 1);
		case 's': case 'S': return Position(x,y + 1);
		case 'a': case 'A': return Position(x - 1,y);
		case 'd': case 'D': return Position(x + 1,y);
		default: return *this;
	}
}

// Diz se as duas posições são a mesma célula
bool Position::equals(const Position& other) const
{
	return x == other.getX() && y == other.getY();
}