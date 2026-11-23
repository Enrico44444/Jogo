#include <algorithm>

#include "Player.h"
#include "WeaponItem.h"
#include "AmmoItem.h"

// Começa desarmado, olhando para a direita
Player::Player(Position pos) : Character(pos), armed(false), facing('d') {}

// Atira a arma equipada. Devolve o dano do tiro, ou 0 se não houver arma ou se ela estiver sem munição
float Player::shoot()
{
	if (!armed)
	{
		return 0;
	}
	return weapon.shoot();
}

// Vira o jogador para a direção dada. Teclas que não são direção são ignoradas
void Player::face(char direction)
{
	switch (direction)
	{
		case 'w': case 'W': facing = 'w'; break;
		case 'a': case 'A': facing = 'a'; break;
		case 's': case 'S': facing = 's'; break;
		case 'd': case 'D': facing = 'd'; break;
		default: break;
	}
}

// Equipa a arma. Quem chama confere antes com hasWeapon: a arma anterior não é guardada aqui
void Player::equipWeapon(const Weapon& newWeapon)
{
	weapon = newWeapon;
	armed = true;
}

// Guarda a arma equipada no inventário como um WeaponItem, levando junto a munição que ela tem no carregador.
// Se o inventário estiver cheio, a arma continua equipada
bool Player::unequipWeapon(std::string& message)
{
	if (!armed)
	{
		message = "Nenhuma arma equipada";
		return false;
	}

	std::unique_ptr<Item> item(new WeaponItem(pos, weapon));
	if (!inventory.canAdd(*item))
	{
		message = "Inventario cheio: nao deu para guardar " + weapon.getName();
		return false;
	}

	message = "Guardou " + weapon.getName();
	inventory.addItem(std::move(item));
	armed = false;
	return true;
}

// Recarrega a arma equipada com a munição do mesmo tipo que está no inventário.
// Entra só o que cabe no carregador, o resto das balas continua no inventário
bool Player::reload(std::string& message)
{
	if (!armed)
	{
		message = "Nenhuma arma equipada";
		return false;
	}
	if (weapon.getAmmo() >= weapon.getMaxAmmo())
	{
		message = "Carregador cheio";
		return false;
	}

	const AmmoItem* ammo = inventory.findAmmo(weapon.getAmmoType());
	if (ammo == nullptr)
	{
		message = "Sem munição para " + weapon.getName();
		return false;
	}

	// O nome é copiado antes de remover: se as balas acabarem, o item é apagado do inventário
	std::string nome = ammo->getName();
	int entrou = weapon.reload(std::min(ammo->getAmount(), weapon.getMaxAmmo() - weapon.getAmmo()));
	inventory.removeAmount(nome, entrou);

	message = "Recarregou " + weapon.getName() + ": +" + std::to_string(entrou) + " balas";
	return true;
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
