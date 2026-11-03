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
	float heal(float amount);   // Cura até o máximo, devolve quanto de vida foi recuperado (0 se já estava cheia)
	bool canPickUp(const Item& item) const;   // Verifica se o inventário tem espaço para o item
	bool pickUpItem(std::unique_ptr<Item> item);   // Guarda o item no inventário (chame canPickUp antes)
	const KeyItem* findKey(const std::string& keyId) const;   // Chave com esse id no inventário, ou nullptr
	bool useKey(const std::string& keyId);   // Consome uma chave com esse id
	bool useItem(const std::string& name, std::string& message);   // Usa o item do inventário e devolve true se o item foi gasto
	const Inventory& getInventory() const { return inventory; }   // Para mostrar o inventário na tela

};
#endif
