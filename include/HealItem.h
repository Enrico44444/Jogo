#ifndef HEAL_ITEM_H
#define HEAL_ITEM_H

#include "Item.h"
#include "Player.h"

class HealItem : public Item
{
private:
	float healAmount;

public:
	void use(Player player);

};

#endif
