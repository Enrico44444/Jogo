#include "HealItem.h"


// Erva com nome e descrição fixos e a quantidade de cura informada
HealItem::HealItem(Position pos, float healAmount) : Item("Erva", "Recupera vida", 1, pos), healAmount(healAmount) {}

// Ainda não implementada
void HealItem::use(Player player)
{
}

// Símbolo da erva no mapa
char HealItem::symbol() const
{
	return 'H';
}
