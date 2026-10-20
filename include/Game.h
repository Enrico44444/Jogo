#ifndef GAME_H
#define GAME_H

#include "Player.h"
#include "MapSection.h"
#include "Door.h"

class Game
{
private:
	Player player;
	MapSection currentSection;
	bool running;   // Enquanto for true, o loop continua

public:
	Game();
	void start();
	void gameLoop();
	void processInput();
	void update();
	void render();
	bool askEnterDoor(Door door);
	void changeSection(Door door);

};

#endif
