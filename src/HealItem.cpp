#include "HealItem.h"


// Erva cujo nome inclui o valor da cura ("Erva +25"): assim, ervas que curam valores diferentes não empilham
HealItem::HealItem(Position pos, float healAmount) : Item("Erva +" + std::to_string(static_cast<int>(healAmount)), "Recupera vida", 1, pos), healAmount(healAmount) {}

// Cura o jogador. Se a vida já está cheia, não gasta a erva (devolve false)
bool HealItem::use(Player& player, std::string& message)
{
	float curou = player.heal(healAmount);
	if (curou <= 0)
	{
		message = "Vida já está cheia";
		return false;
	}

	message = "Usou " + getName() + ": curou " + std::to_string(static_cast<int>(curou)) + " de vida";
	return true;
}

// Símbolo da erva no mapa
char HealItem::symbol() const
{
	return 'H';
}