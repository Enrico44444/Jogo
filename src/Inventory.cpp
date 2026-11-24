#include <algorithm>
#include <string>
#include <utility>

#include "Inventory.h"
#include "Player.h"
#include "KeyItem.h"
#include "AmmoItem.h"

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
	// find_if devolve um iterador para o primeiro item em que a lambda dá true (ou items.end() se nenhum der).
	// [&] deixa a lambda usar o "item" desta função
	auto existente = std::find_if(items.begin(), items.end(),
		[&](const std::unique_ptr<Item>& i) { return i->getName() == item->getName(); });
	if (existente != items.end())
	{
		(*existente)->addAmount(item->getAmount());
		return true;
	}

	// Item novo: ocupa uma vaga
	items.push_back(std::move(item));
	return true;
}

// Tira uma unidade do item com esse nome, se a quantidade chega a zero, a vaga é liberada
bool Inventory::removeItem(std::string name)
{
	return removeAmount(name, 1);
}

// Verifica se o inventário está cheio
bool Inventory::isFull() const
{
	return static_cast<int>(items.size()) >= maxSize;
}

// Verifica se o item está no inventário
bool Inventory::hasItem(std::string name) const
{
	// any_of devolve true se a lambda der true para algum item
	return std::any_of(items.begin(), items.end(),
		[&](const std::unique_ptr<Item>& item) { return item->getName() == name; });
}

// Procura no inventário uma chave com esse id. Só as KeyItem têm id, então os outros itens são ignorados.
// Devolve nullptr se não houver.
const KeyItem* Inventory::findKey(const std::string& keyId) const
{
	// dynamic_cast devolve nullptr quando o item não é uma KeyItem
	auto achado = std::find_if(items.begin(), items.end(),
		[&](const std::unique_ptr<Item>& item)
		{
			const KeyItem* key = dynamic_cast<const KeyItem*>(item.get());
			return key != nullptr && key->getKeyId() == keyId;
		});
	if (achado == items.end())
	{
		return nullptr;
	}
	return static_cast<const KeyItem*>(achado->get());
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

// Procura no inventário a munição desse tipo. Só as AmmoItem têm tipo, então os outros itens são ignorados.
// Devolve nullptr se não houver.
const AmmoItem* Inventory::findAmmo(AmmoType type) const
{
	// dynamic_cast devolve nullptr quando o item não é uma AmmoItem
	auto achado = std::find_if(items.begin(), items.end(),
		[&](const std::unique_ptr<Item>& item)
		{
			const AmmoItem* ammo = dynamic_cast<const AmmoItem*>(item.get());
			return ammo != nullptr && ammo->getType() == type;
		});
	if (achado == items.end())
	{
		return nullptr;
	}
	return static_cast<const AmmoItem*>(achado->get());
}

// Tira n unidades do item com esse nome (para a munição, n é o número de balas).
// Se a quantidade chega a zero, a vaga é liberada. Devolve false se não houver o item
bool Inventory::removeAmount(std::string name, int n)
{
	auto achado = std::find_if(items.begin(), items.end(),
		[&](const std::unique_ptr<Item>& item) { return item->getName() == name; });
	if (achado == items.end())
	{
		return false;
	}

	(*achado)->removeAmount(n);
	if ((*achado)->getAmount() <= 0)
	{
		items.erase(achado);
	}
	return true;
}

// Usa o item com esse nome: o próprio item decide o efeito e escreve em message o que aconteceu.
// Se o item foi gasto, tira uma unidade (a vaga é liberada quando a quantidade chega a zero).
// Devolve true se o item foi gasto
bool Inventory::useItem(std::string name, Player& player, std::string& message)
{
	auto achado = std::find_if(items.begin(), items.end(),
		[&](const std::unique_ptr<Item>& item) { return item->getName() == name; });
	if (achado == items.end())
	{
		message = "Voce não tem " + name;
		return false;
	}

	bool gasto = (*achado)->use(player, message);
	if (gasto)
	{
		removeItem(name);
	}
	return gasto;
}