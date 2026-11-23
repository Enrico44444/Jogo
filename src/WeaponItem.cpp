#include "WeaponItem.h"


// Nome do item com a munição do carregador ("Pistola 12/12"): assim, armas com carregadores diferentes
// não empilham no inventário (mesmo padrão da "Erva +25")
static std::string nomeArma(const Weapon& weapon)
{
	return weapon.getName() + " " + std::to_string(weapon.getAmmo()) + "/" + std::to_string(weapon.getMaxAmmo());
}

WeaponItem::WeaponItem(Position pos, Weapon weapon)
	: Item(nomeArma(weapon), "Arma de fogo", 1, pos), weapon(weapon) {}

// Equipa a arma. Se o jogador já tem uma equipada, não troca: ele precisa guardar a atual antes (tecla g).
// Devolve true se a arma foi equipada (o item sai do inventário, a arma agora está na mão do jogador)
bool WeaponItem::use(Player& player, std::string& message)
{
	if (player.hasWeapon())
	{
		message = "Já há uma arma equipada: guarde-a com g";
		return false;
	}

	player.equipWeapon(weapon);
	message = "Equipou " + weapon.getName();
	return true;
}

// Símbolo da arma no mapa
char WeaponItem::symbol() const
{
	return 'W';
}
