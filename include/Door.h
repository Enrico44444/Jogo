#ifndef DOOR_H
#define DOOR_H

#include <string>

#include "Tile.h"
#include "Position.h"
#include "KeyItem.h"
#include "Player.h"

// Porta que leva a outra sala. É um Tile, mas não é andável como o piso.
class Door : public Tile
{
private:
	bool locked;  
	bool open;
	std::string requiredKey;   // Id da chave que abre a porta (vazio = sem chave)
	std::string targetFile;   // Arquivo da sala de destino
	Position targetPos;   // Onde o jogador aparece na sala de destino

public:
	Door();   // Porta vazia, sem destino
	Door(Position pos);   // Porta na posição dada, ainda sem destino
	
	// Define o destino e a chave da porta (chamado pelo loadFromFile)
	void configure(std::string targetFile, Position targetPos, std::string requiredKey);

	bool isLocked();   // Ainda não implementada
	bool unlock(KeyItem key);   // Ainda não implementada
	bool use(Player player);   // Ainda não implementada

	std::string getTargetFile() const { return targetFile; }   // Arquivo da sala de destino
	Position getTargetPos() const { return targetPos; }   // Onde o jogador aparece na sala de destino
	char symbol() const override { return 'D'; }   // Desenhada como D

};

#endif
