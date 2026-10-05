#ifndef ITEM_H
#define ITEM_H

#include <string>

#include "Position.h"
#include "Player.h"

class Item
{
private:
	std::string name;
	std::string description;
	int amount;
	Position pos;

public:
	void use(Player player);
	void addAmount(int n);
	void removeAmount(int n);

};

#endif
