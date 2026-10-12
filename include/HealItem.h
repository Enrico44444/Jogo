#ifndef HEAL_ITEM_H
#define HEAL_ITEM_H

#include "Item.h"
#include "Player.h"

// Erva: cura o jogador ao ser usada.
class HealItem : public Item
{
private:
	float healAmount;

public:
	HealItem(Position pos, float healAmount);
	void use(Player player) override;   // Ainda não implementada
	char symbol() const override;   // Desenhada como H
};

#endif
