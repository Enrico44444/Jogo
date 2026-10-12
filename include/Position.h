#ifndef POSITION_H
#define POSITION_H

// Posição de uma célula da sala: coluna (x) e linha (y), a partir de 0 no canto superior esquerdo.
class Position
{
private:
	int x;
	int y;

public:
	Position();   // Posição (0, 0)
	Position(int x, int y);   // Posição na coluna x e linha y
	void move(int x, int y);   // Soma o deslocamento à posição atual
	int getX() const;
	int getY() const;
	Position nextPos(char direction) const;   // Posição vizinha na direção dada, sem alterar esta
	bool equals(const Position& other) const;   // True se for a mesma célula

};

#endif
