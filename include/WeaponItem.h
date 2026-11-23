#ifndef WEAPON_ITEM_H
#define WEAPON_ITEM_H

#include "Item.h"
#include "Player.h"
#include "Weapon.h"

// Arma no chão ou no inventário: ao ser usada, o jogador a equipa.
class WeaponItem : public Item
{
private:
	Weapon weapon;   // A arma em si, com a munição que ela tem no carregador

public:
	WeaponItem(Position pos, Weapon weapon);   // Arma na posição dada
	const Weapon& getWeapon() const { return weapon; }
	bool use(Player& player, std::string& message) override;   // Equipa a arma (só se o jogador não tiver outra equipada)
	char symbol() const override;   // Desenhada como W

};

#endif
