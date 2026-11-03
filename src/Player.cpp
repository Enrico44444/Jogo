#include "Player.h"

Player::Player(Position pos) : Character(pos) {}

float Player::shoot()
{
	return 0;
}

// Cura o jogador sem passar da vida máxima. Devolve quanto de vida foi recuperado (0 se já estava cheia)
float Player::heal(float amount)
{
	if (amount <= 0)
	{
		return 0;
	}

	float antes = life;
	life += amount;
	if (life > maxLife)
	{
		life = maxLife;
	}
	return life - antes;
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

// Usa um item do inventário. O próprio item decide o efeito e escreve a mensagem do resultado.
// Devolve true se o item foi gasto
bool Player::useItem(const std::string& name, std::string& message)
{
	return inventory.useItem(name, *this, message);
}

// Guarda o item no inventário
bool Player::pickUpItem(std::unique_ptr<Item> item)
{
	return inventory.addItem(std::move(item));
}
