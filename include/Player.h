#ifndef PLAYER_H
#define PLAYER_H

#include "Character.h"
#include "Inventory.h"
#include "Weapon.h"
#include "Item.h"

class Player : public Character
{
private:
	Inventory inventory;
	Weapon weapon;

public:
	Player(Position pos);
	float shoot();
	void heal(float amount);
	bool canPickUp(const Item& item) const;   // Verifica se o inventário tem espaço para o item
	bool pickUpItem(std::unique_ptr<Item> item);   // Guarda o item no inventário (chame canPickUp antes)
	const Inventory& getInventory() const { return inventory; }   // Para mostrar o inventário na tela

};
#endif
