#include "Item.h"
#include "Player.h"

// Item vazio
Item::Item() : name(""), description(""), amount(0), pos() {}

// Item com nome, descrição, quantidade e posição
Item::Item(std::string name, std::string description, int amount, Position pos)
    : name(name), description(description), amount(amount), pos(pos) {}

// Efeito padrão: nenhum (as subclasses redefinem)
void Item::use(Player player)
{
}

// Símbolo padrão; as subclasses devolvem o seu
char Item::symbol() const
{
	return '?';
}

Position Item::getPos() const{
    return pos;
}