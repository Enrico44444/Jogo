#ifndef INVENTORY_H
#define INVENTORY_H

#include <string>
#include <vector>

#include "Item.h"
#include "Player.h"

class Inventory
{
private:
	std::vector<Item> items;
	int maxSize;
	Item item;

public:
	bool addItem(Item item);
	bool removeItem(std::string name);
	bool isFull();
	bool hasItem(std::string name);
	bool useItem(std::string name, Player player);

};

#endif
