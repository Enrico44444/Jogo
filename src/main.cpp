#include "Game.h"

int main()
{
	MapSection map;
	map.loadFromFile("MapSections/map1.txt");
	map.render();
	Game game;
	game.start();
	return 0;
}
