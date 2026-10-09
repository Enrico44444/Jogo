#include <string>

#include "MapSection.h"


bool MapSection::loadFromFile(std::string path)
{
	return false;
}

void MapSection::update(Player player)
{
}

void MapSection::render()
{
}

void MapSection::resetZombies()
{
}

Tile MapSection::getTileAt(Position pos)
{
	return Tile();
}

bool MapSection::isWalkable(Position pos)
{
	return false;
}

Door MapSection::getDoorAt(Position pos)
{
	return Door();
}

Zombie MapSection::getZombieAt(Position pos)
{
	return Zombie();
}

Item MapSection::getItemAt(Position pos)
{
	return Item();
}

MapSection::MapSection(std::string filePath)
{
}
