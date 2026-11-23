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
	float messageTimer;   // Quanto tempo (em segundos) falta para a mensagem sumir
	float moveCooldown;   // Tempo (em segundos) até o jogador poder dar o próximo passo
	float shootCooldown;   // Tempo (em segundos) até o jogador poder atirar de novo
	bool needsRedraw;   // true quando algo mudou e a tela precisa ser atualizada

public:
	Game();
	void start();
	void gameLoop();
	void processInput();   // Lê as teclas que chegaram (sem esperar) e trata cada uma
	void handleKey(char key);   // Trata uma tecla (separada do processInput para poder ser testada)
	void update(float dt);   // Avança o tempo: dt = segundos desde o quadro anterior
	void render();
	bool askEnterDoor(Door door);
	bool unlockDoor(Door& door, Position pos);   // Garante que a porta pode ser atravessada (usa a chave, se estiver trancada)
	void changeSection(Door door);
	void pickUpItemHere();   // Pega o item que estiver na posição do jogador, se couber no inventário
	void shootWeapon();   // Atira na direção em que o jogador olha e dá o dano ao primeiro zumbi no caminho
	void useInventoryItem(int number);   // Usa o item de número "number" da lista do inventário
	void showMessage(const std::string& text);   // Mostra uma mensagem abaixo do mapa por alguns segundos

};

#endif
