#include <string>
#include <vector>
#include <list>
#include <iostream>

#include "Door.h"

// Porta vazia: não andável, destrancada e sem destino
Door::Door() : Tile(), locked(false), open(false)
{
}

// Porta na posição dada, ainda sem destino
Door::Door(Position pos) : Tile(pos, false), locked(false), open(false)
{
}

// Define o destino (arquivo e posição de chegada) e a chave da porta.
// Se houver chave, a porta começa trancada.
void Door::configure(std::string targetFile, Position targetPos, std::string requiredKey)
{
	this->targetFile = targetFile;
	this->targetPos = targetPos;
	this->requiredKey = requiredKey;
	locked = !requiredKey.empty();
}

bool Door::isLocked()
{
	return false;
}

bool Door::unlock(KeyItem key)
{
	return false;
}

bool Door::use(Player player)
{
	return false;
}
