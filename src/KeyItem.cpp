#include "KeyItem.h"


// Chave cujo nome inclui o id, para o inventário não juntar chaves diferentes
KeyItem::KeyItem(Position pos, std::string keyId)
	: Item("Chave " + keyId, "Abre uma porta trancada", 1, pos), keyId(keyId) {}

// Ainda não implementada
void KeyItem::use(Player player)
{
}

// Símbolo da chave no mapa
char KeyItem::symbol() const
{
	return 'K';
}
