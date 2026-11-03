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

// Diz se a porta está trancada
bool Door::isLocked() const
{
	return locked;
}

// Destranca a porta se a chave tiver o mesmo id que a porta exige. Devolve true se destrancou.
// (O requiredKey continua guardado: o Game usa o id para a mensagem)
bool Door::unlock(const KeyItem& key)
{
	if (key.getKeyId() != requiredKey)
	{
		return false;
	}

	locked = false;
	return true;
}

bool Door::use(Player& player)
{
	return false;
}
