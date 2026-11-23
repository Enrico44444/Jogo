#ifndef PLAYER_H
#define PLAYER_H

#include "Character.h"
#include "Inventory.h"
#include "Weapon.h"
#include "Item.h"

class Player : public Character
{
private:
	Inventory inventory;
	Weapon weapon;   // A arma equipada (só vale se armed for true)
	bool armed;   // O jogador tem uma arma equipada?
	char facing;   // Para onde o jogador olha (w, a, s ou d): é a direção do tiro

public:
	Player(Position pos);
	float shoot();   // Atira a arma equipada e devolve o dano (0 se não houver arma ou munição)
	void face(char direction);   // Vira o jogador para a direção dada (w, a, s ou d); outra tecla é ignorada
	char getFacing() const { return facing; }   // Direção para onde o jogador olha
	bool hasWeapon() const { return armed; }   // Há uma arma equipada?
	const Weapon& getWeapon() const { return weapon; }   // A arma equipada (chame hasWeapon antes)
	void equipWeapon(const Weapon& newWeapon);   // Equipa a arma (chame hasWeapon antes: não guarda a anterior)
	bool unequipWeapon(std::string& message);   // Guarda a arma equipada no inventário, com a munição que ela tem. Devolve true se guardou
	bool reload(std::string& message);   // Recarrega a arma equipada com a munição do inventário. Devolve true se entrou alguma bala
	float heal(float amount);   // Cura até o máximo, devolve quanto de vida foi recuperado (0 se já estava cheia)
	bool canPickUp(const Item& item) const;   // Verifica se o inventário tem espaço para o item
	bool pickUpItem(std::unique_ptr<Item> item);   // Guarda o item no inventário (chame canPickUp antes)
	const KeyItem* findKey(const std::string& keyId) const;   // Chave com esse id no inventário, ou nullptr
	bool useKey(const std::string& keyId);   // Consome uma chave com esse id
	bool useItem(const std::string& name, std::string& message);   // Usa o item do inventário e devolve true se o item foi gasto
	const Inventory& getInventory() const { return inventory; }   // Para mostrar o inventário na tela

};
#endif
