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
	virtual bool use(Player& player, std::string& message);   // Efeito de usar o item, devolve true se o item foi gasto. Cada subclasse redefine
	void addAmount(int n);   // Soma n à quantidade
	void removeAmount(int n);   // Tira n da quantidade
	Position getPos() const;
	std::string getName() const { return name; }   // Nome do item (usado para empilhar no inventário)
	int getAmount() const { return amount; }   // Quantidade empilhada
	virtual char symbol() const;   // Caractere usado para desenhar o item; cada subclasse redefine

};

#endif
