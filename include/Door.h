#ifndef DOOR_H
#define DOOR_H

#include <string>

#include "Tile.h"
#include "Position.h"
#include "KeyItem.h"
#include "Player.h"

class Door : public Tile
{
private:
	bool locked;
	bool open;
	std::string requiredKey;
	std::string targetFile;
	Position targetPos;

public:
	bool isLocked();
	bool unlock(KeyItem key);
	bool use(Player player);

};

#endif
