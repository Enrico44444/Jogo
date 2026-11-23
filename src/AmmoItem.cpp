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

// Munição com nome conforme o tipo. A quantidade do item é o número de balas, então pilhas do mesmo tipo somam as balas
AmmoItem::AmmoItem(Position pos, AmmoType type, int ammoAmount)
	: Item(nomeMunicao(type), "Municao para a arma", ammoAmount, pos), type(type) {}

// A munição só é gasta ao recarregar a arma (tecla r, Player::reload): aqui só avisa e não gasta
bool AmmoItem::use(Player& player, std::string& message)
{
	message = "A munição é usada ao recarregar a arma: tecla r";
	return false;
}

// Símbolo da munição no mapa
char AmmoItem::symbol() const
{
	return 'A';
}
