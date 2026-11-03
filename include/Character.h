#ifndef CHARACTER_H
#define CHARACTER_H

#include "Position.h"
#include "MoveResult.h"

class MapSection;

class Character
{
protected:
	float life;
	float maxLife;   // Vida máxima: a cura não passa disso
	Position pos;

public:
	Character(Position pos);
	MoveResult move(char direction, MapSection& section);
	void takeDamage(float amount);
	bool isAlive() const;   // Está vivo? (vida maior que zero)
	float getLife() const { return life; }   // Vida atual
	float getMaxLife() const { return maxLife; }   // Vida máxima
	Position getPos() const { return pos; }
	void setPos(Position newPos) { pos = newPos; }   // Coloca o personagem em outra posição (usado na troca de sala)

};

#endif
