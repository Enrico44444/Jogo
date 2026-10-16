#ifndef ITEM_H
#define ITEM_H

#include <string>
#include "Position.h"

class Player;

// Base dos itens do jogo (erva, munição, chave).
class Item
{
private:
	std::string name;
	std::string description;
	int amount;
	Position pos;

public:
	Item();   // Item vazio
	Item(std::string name, std::string description, int amount, Position pos);   // Item com os dados completos
	virtual ~Item() = default;   // Virtual: permite apagar um HealItem, AmmoItem ou KeyItem por um ponteiro Item*
	virtual void use(Player player);   // Efeito de usar o item; cada subclasse redefine
	void addAmount(int n);   // Ainda não implementada
	void removeAmount(int n);   // Ainda não implementada
	Position getPos() const;
	virtual char symbol() const;   // Caractere usado para desenhar o item; cada subclasse redefine

};

#endif
