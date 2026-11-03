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
	bool unlockDoor(Door& door, Position pos);   // Garante que a porta pode ser atravessada (usa a chave, se estiver trancada)
	void changeSection(Door door);
	void pickUpItemHere();   // Pega o item que estiver na posição do jogador, se couber no inventário
	void useInventoryItem();   // Pergunta qual item usar (pelo número da lista do inventário) e usa

};

#endif
