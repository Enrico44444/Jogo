#ifndef GAME_H
#define GAME_H

#include <string>

#include "Player.h"
#include "MapSection.h"
#include "Door.h"

class Game
{
private:
	Player player;
	MapSection currentSection;
	bool running;   // Enquanto for true, o loop continua
	std::string message;   // Mensagem mostrada abaixo do mapa

public:
	Game();
	void start();
	void gameLoop();
	void processInput();
	void update();
	void render();
	bool askEnterDoor(Door door);
	void changeSection(Door door);
	void pickUpItemHere();   // Pega o item que estiver na posição do jogador, se couber no inventário

};

#endif
