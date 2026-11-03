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
	bool use(Player& player, std::string& message) override;   // Cura o jogador, mas não gasta a erva se a vida já estiver cheia
	char symbol() const override;   // Desenhada como H
};

#endif
