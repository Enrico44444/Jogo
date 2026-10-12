#ifndef MAP_SECTION_H
#define MAP_SECTION_H

#include <memory>
#include <string>
#include <vector>

#include "Tile.h"
#include "Zombie.h"
#include "Item.h"
#include "Door.h"
#include "Player.h"
#include "Position.h"


// Uma sala do jogo, montada a partir de um arquivo .txt.
class MapSection
{
private:
	int mapX;
	int mapY;
	std::vector<std::unique_ptr<Tile>> tiles;   // Armazenamento dos tiles da sala; cada tile é um ponteiro para uma subclasse (Floor, Wall ou Door)
	std::vector<Zombie> zombies;   // Zumbis da sala
	std::vector<std::unique_ptr<Item>> items;   // Itens no chão
	std::vector<Door*> doors;                   // Portas ficam em tiles, mas também aqui para facilitar a busca
	Position playerStart;   // Onde o jogador começa nesta sala

public:
	bool loadFromFile(std::string path);   // Lê o arquivo da sala; devolve false se houver erro
	void update(Player player);   // Ainda não implementado
	void render();   // Ainda não implementado
	void resetZombies();   // Ainda não implementado
	Tile getTileAt(Position pos);   // Ainda não implementado
	bool isWalkable(Position pos);   // Ainda não implementado
	Door getDoorAt(Position pos);   // Ainda não implementado
	Zombie getZombieAt(Position pos);   // Ainda não implementado
	Item getItemAt(Position pos);   // Ainda não implementado
	MapSection(std::string filePath = "");   // Se receber um caminho, já carrega a sala

	Position getPlayerStart() const { return playerStart; }   // Onde o jogador começa nesta sala

};
#endif
