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
	std::vector<char> lastFrame;   // Último desenho feito na tela, o update compara com ele
	std::vector<char> buildFrame(Position playerPos) const;   // Monta o desenho do mapa, mas não imprime
	void draw(Position playerPos);   // Limpa a tela e imprime o mapa inteiro

public:
	bool loadFromFile(std::string path);   // Lê o arquivo da sala; devolve false se houver erro
	void update(const Player& player);   // Redesenha só as células que mudaram desde o último desenho
	int getMapX() const;
	int getMapY() const;
	void render();   // Imprime o mapa inteiro pela primeira vez (limpa a tela antes)
	void resetZombies();   // Ainda não implementado
	Tile* getTileAt(Position pos);   // Tile da posição dada, ou nullptr se estiver fora da sala
	bool isWalkable(Position pos);   // Ainda não implementado
	Door* getDoorAt(Position pos);   // Porta da posição dada, ou nullptr se não houver porta ali
	Zombie getZombieAt(Position pos);   // Ainda não implementado
	Item getItemAt(Position pos);   // Ainda não implementado
	MapSection(std::string filePath = "");   // Se receber um caminho, já carrega a sala

	Position getPlayerStart() const { return playerStart; }   // Onde o jogador começa nesta sala

};
#endif
