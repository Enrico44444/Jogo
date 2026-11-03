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

// A munição só é usada ao recarregar a arma (ainda não implementado): por enquanto só avisa e não gasta
bool AmmoItem::use(Player& player, std::string& message)
{
	message = "A munição é usada ao recarregar a arma";
	return false;
}

// Símbolo da munição no mapa
char AmmoItem::symbol() const
{
	return 'A';
}
