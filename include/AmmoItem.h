#ifndef AMMO_ITEM_H
#define AMMO_ITEM_H

#include "Item.h"
#include "Player.h"

class AmmoItem : public Item
{
private:
	int ammoAmount;

public:
	void use(Player player);

};

#endif
