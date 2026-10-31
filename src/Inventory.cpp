#include <string>
#include <utility>

#include "Inventory.h"
#include "Player.h"
#include "KeyItem.h"

// Inventário vazio com 8 vagas
Inventory::Inventory() : maxSize(8)
{
}

// Cabe se já existe uma pilha com o mesmo nome (a quantidade só soma) ou se ainda há vaga
bool Inventory::canAdd(const Item& item) const
{
	return hasItem(item.getName()) || !isFull();
}

// Guarda o item. Se já existe um com o mesmo nome, soma a quantidade e descarta o novo.
// Devolve false (e o item é perdido) se não couber: por isso canAdd é chamado antes.
bool Inventory::addItem(std::unique_ptr<Item> item)
{
	if (item == nullptr || !canAdd(*item))
	{
		return false;
	}

	// Já existe uma pilha com esse nome: só soma
	for (std::unique_ptr<Item>& existente : items)
	{
		if (existente->getName() == item->getName())
		{
			existente->addAmount(item->getAmount());
			return true;
		}
	}

	// Item novo: ocupa uma vaga
	items.push_back(std::move(item));
	return true;
}

// Tira uma unidade do item com esse nome, se a quantidade chega a zero, a vaga é liberada
bool Inventory::removeItem(std::string name)
{
	for (std::size_t i = 0; i < items.size(); i++)
	{
		if (items[i]->getName() == name)
		{
			items[i]->removeAmount(1);
			if (items[i]->getAmount() <= 0)
			{
				items.erase(items.begin() + i);
			}
			return true;
		}
	}
	return false;
}

// Verifica se o inventário está cheio
bool Inventory::isFull() const
{
	return static_cast<int>(items.size()) >= maxSize;
}

// Verifica se o item está no inventário
bool Inventory::hasItem(std::string name) const
{
	for (const std::unique_ptr<Item>& item : items)
	{
		if (item->getName() == name)
		{
			return true;
		}
	}
	return false;
}

// Procura no inventário uma chave com esse id. Só as KeyItem têm id, então os outros itens são ignorados.
// Devolve nullptr se não houver.
const KeyItem* Inventory::findKey(const std::string& keyId) const
{
	for (const std::unique_ptr<Item>& item : items)
	{
		// dynamic_cast devolve nullptr quando o item não é uma KeyItem
		const KeyItem* key = dynamic_cast<const KeyItem*>(item.get());
		if (key != nullptr && key->getKeyId() == keyId)
		{
			return key;
		}
	}
	return nullptr;
}

// Consome uma unidade da chave com esse id. Devolve false se não houver.
bool Inventory::removeKey(const std::string& keyId)
{
	const KeyItem* key = findKey(keyId);
	if (key == nullptr)
	{
		return false;
	}

	// O nome é copiado antes de remover: se a quantidade chegar a zero, o item é apagado da lista
	return removeItem(key->getName());
}

// Ainda não implementada
bool Inventory::useItem(std::string name, Player player)
{
	return false;
}
