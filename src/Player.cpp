#include "Player.h"

Player::Player(Position pos) : Character(pos) {}

float Player::shoot()
{
	return 0;
}

void Player::heal(float amount)
{
}

// Verifica se o inventário tem espaço para o item
bool Player::canPickUp(const Item& item) const
{
	return inventory.canAdd(item);
}

// Guarda o item no inventário
bool Player::pickUpItem(std::unique_ptr<Item> item)
{
	return inventory.addItem(std::move(item));
}
