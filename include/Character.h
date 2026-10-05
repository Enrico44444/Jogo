#ifndef CHARACTER_H
#define CHARACTER_H

#include "Position.h"
#include "MoveResult.h"
#include "MapSection.h"

class Character
{
private:
	Position position;

protected:
	float life;
	Position pos;

public:
	MoveResult move(char direction, MapSection section);
	void takeDamage(float amount);
	bool isAlive();

};

#endif
