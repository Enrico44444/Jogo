#ifndef CHARACTER_H
#define CHARACTER_H

#include "Position.h"
#include "MoveResult.h"

class MapSection;

class Character
{
protected:
	float life;
	Position pos;

public:
	Character(Position pos);
	MoveResult move(char direction, MapSection& section);
	void takeDamage(float amount);
	bool isAlive();

};

#endif
