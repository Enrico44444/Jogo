#ifndef AMMO_ITEM_H
#define AMMO_ITEM_H

#include "Item.h"
#include "Player.h"
#include "AmmoType.h"

// Munição: recarrega a arma do tipo correspondente.
class AmmoItem : public Item
{
private:
	AmmoType type;
	int ammoAmount;

public:
	AmmoItem(Position pos, AmmoType type, int ammoAmount);   // Munição na posição dada
	bool use(Player& player, std::string& message) override;   // A munição é usada ao recarregar a arma: aqui só avisa
	char symbol() const override;   // Desenhada como A

};

#endif
