#ifndef INVENTORY_H
#define INVENTORY_H

#include <memory>
#include <string>
#include <vector>

#include "Item.h"

class Player;
class KeyItem;

// Inventário do jogador. Itens com o mesmo nome ficam empilhados em uma única vaga.
class Inventory
{
private:
	std::vector<std::unique_ptr<Item>> items;   // Uma vaga por item diferente, a quantidade fica em Item::amount
	int maxSize;   // Número de vagas

public:
	Inventory();   // Inventário vazio com 8 vagas

	bool canAdd(const Item& item) const;   // Verifica se tem espaço
	bool addItem(std::unique_ptr<Item> item);   // Guarda o item, somando à pilha se já existir. Chama canAdd antes.
	bool removeItem(std::string name);   // Tira uma unidade do item com esse nome
	bool isFull() const;   // Verifica se o inventário está cheio
	bool hasItem(std::string name) const;   // Verifica se tem um item específico
	const KeyItem* findKey(const std::string& keyId) const;   // Procura uma chave pelo id; nullptr se não tiver
	bool removeKey(const std::string& keyId);   // Consome uma unidade da chave com esse id
	bool useItem(std::string name, Player& player, std::string& message);   // Usa o item com esse nome e devolve true se o item foi gasto

	const std::vector<std::unique_ptr<Item>>& getItems() const { return items; }   // Para mostrar o inventário na tela
	int getMaxSize() const { return maxSize; }   // Número de vagas
};

#endif
