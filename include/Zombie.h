#ifndef ZOMBIE_H
#define ZOMBIE_H

#include "Character.h"
#include "Position.h"
#include "Player.h"

class Zombie : public Character
{
private:
	float damage;
	Position startPos;

public:
	void update(Player target);
	void attack(Player target);
	void resetPosition();

};

#endif
