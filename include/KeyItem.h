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
	bool use(Player& player, std::string& message) override;   // A chave abre a porta sozinha, aqui só avisa
	char symbol() const override;   // Desenhada como K

};

#endif
