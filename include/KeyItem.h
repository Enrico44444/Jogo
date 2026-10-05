#ifndef KEY_ITEM_H
#define KEY_ITEM_H

#include <string>

#include "Item.h"
#include "Player.h"

class KeyItem : public Item
{
private:
	std::string keyId;

public:
	void use(Player player);

};

#endif
