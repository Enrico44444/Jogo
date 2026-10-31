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

// Chave com esse id no inventário, ou nullptr se o jogador não tiver
const KeyItem* Player::findKey(const std::string& keyId) const
{
	return inventory.findKey(keyId);
}

// Consome uma unidade da chave com esse id
bool Player::useKey(const std::string& keyId)
{
	return inventory.removeKey(keyId);
}

// Guarda o item no inventário
bool Player::pickUpItem(std::unique_ptr<Item> item)
{
	return inventory.addItem(std::move(item));
}
