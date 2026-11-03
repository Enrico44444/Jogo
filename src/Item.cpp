#include "Item.h"
#include "Player.h"

// Item vazio
Item::Item() : name(""), description(""), amount(0), pos() {}

// Item com nome, descrição, quantidade e posição
Item::Item(std::string name, std::string description, int amount, Position pos)
    : name(name), description(description), amount(amount), pos(pos) {}

// Efeito padrão: nenhum (as subclasses redefinem)
bool Item::use(Player& player, std::string& message)
{
	message = "Não é possível usar " + name;
	return false;
}

// Símbolo padrão; as subclasses devolvem o seu
char Item::symbol() const
{
	return '?';
}

Position Item::getPos() const{
    return pos;
}

// Soma n à quantidade
void Item::addAmount(int n)
{
	amount += n;
}

// Tira n da quantidade, sem deixar ficar negativa
void Item::removeAmount(int n)
{
	amount -= n;
	if (amount < 0)
	{
		amount = 0;
	}
}
