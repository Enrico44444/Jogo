#include "KeyItem.h"


// Chave cujo nome inclui o id, para o inventário não juntar chaves diferentes
KeyItem::KeyItem(Position pos, std::string keyId)
	: Item("Chave " + keyId, "Abre uma porta trancada", 1, pos), keyId(keyId) {}

// A chave é usada quando o jogador anda até a porta trancada: aqui só avisa e não gasta
bool KeyItem::use(Player& player, std::string& message)
{
	message = "A chave abre a porta sozinha: Vá até ela";
	return false;
}

// Símbolo da chave no mapa
char KeyItem::symbol() const
{
	return 'K';
}
