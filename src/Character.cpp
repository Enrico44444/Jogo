#include "Character.h"
#include "MapSection.h"


MoveResult Character::move(char direction, MapSection& section)
{
	return MoveResult::BLOCKED;
}

void Character::takeDamage(float amount)
{
}

bool Character::isAlive()
{
	return false;
}
