#ifndef MAP_SECTION_H
#define MAP_SECTION_H

#include <string>
#include <vector>

#include "Tile.h"
#include "Zombie.h"
#include "Item.h"
#include "Door.h"
#include "Player.h"
#include "Position.h"

class MapSection
{
private:
	int mapX;
	int mapY;
	std::vector<Tile> tiles;
	std::vector<Zombie> zombies;
	std::vector<Item> items;
	std::vector<Door> doors;
	std::vector<Tile> tile;
	std::vector<Zombie> zombie;
	std::vector<Item> item;
	std::vector<Door> door;

public:
	bool loadFromFile(std::string path);
	void update(Player player);
	void render();
	void resetZombies();
	Tile getTileAt(Position pos);
	bool isWalkable(Position pos);
	Door getDoorAt(Position pos);
	Zombie getZombieAt(Position pos);
	Item getItemAt(Position pos);
	MapSection(std::string filePath = "");

};
#endif
