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
}

bool MapSection::isWalkable(Position pos)
{
}

Door MapSection::getDoorAt(Position pos)
{
}

Zombie MapSection::getZombieAt(Position pos)
{
}

Item MapSection::getItemAt(Position pos)
{
}

MapSection::MapSection(std::string filePath)
{
}
