#ifndef PLAYER_H
#define PLAYER_H

#include "Character.h"
#include "Inventory.h"
#include "Weapon.h"
#include "Item.h"

class Player : public Character
{
private:
	Inventory inventory;
	Weapon weapon;

public:
	Player(Position pos);
	float shoot();
	void heal(float amount);
	bool pickUpItem(Item item);

};
#endif
