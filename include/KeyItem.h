#ifndef KEY_ITEM_H
#define KEY_ITEM_H

#include <string>

#include "Item.h"
#include "Player.h"

// Chave: abre a porta trancada que exige o mesmo id.
class KeyItem : public Item
{
private:
	std::string keyId;   // Identificador da chave

public:
	KeyItem(Position pos, std::string keyId);
	std::string getKeyId() const { return keyId; }   // Id da chave (o mesmo da linha DOOR do mapa)
	void use(Player player) override;   // Ainda não implementada
	char symbol() const override;   // Desenhada como K

};

#endif
