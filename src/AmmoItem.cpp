#include "AmmoItem.h"


// Nome do item conforme o tipo. Cada tipo tem um nome diferente para que o inventário
// não empilhe munições de armas diferentes.
static std::string nomeMunicao(AmmoType type)
{
	if (type == AmmoType::SHOTGUN)
	{
		return "Municao Escopeta";
	}
	if (type == AmmoType::RIFLE)
	{
		return "Municao Rifle";
	}
	return "Municao Pistola";
}

// Munição com nome conforme o tipo e a quantidade de balas informada
AmmoItem::AmmoItem(Position pos, AmmoType type, int ammoAmount)
	: Item(nomeMunicao(type), "Municao para a arma", 1, pos), type(type), ammoAmount(ammoAmount) {}

// Ainda não implementada
void AmmoItem::use(Player player)
{
}

// Símbolo da munição no mapa
char AmmoItem::symbol() const
{
	return 'A';
}
