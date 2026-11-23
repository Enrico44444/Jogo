#ifndef WEAPON_H
#define WEAPON_H

#include <string>

#include "AmmoType.h"

// Arma do jogador: tem dano, alcance e munição no carregador. Cada tipo de munição corresponde a uma arma.
class Weapon
{
private:
	std::string name;
	AmmoType ammoType;   // Tipo de munição que a arma usa
	float damage;   // Dano de cada tiro
	int range;   // Alcance do tiro, em células
	float fireInterval;   // Tempo mínimo entre dois tiros (em segundos)
	int ammo;   // Balas no carregador
	int maxAmmo;   // Capacidade do carregador

public:
	Weapon();   // Pistola com o carregador cheio
	Weapon(AmmoType type);   // Arma do tipo dado com o carregador cheio
	Weapon(AmmoType type, int ammo);   // Arma do tipo dado com "ammo" balas no carregador (usada ao guardar a arma no inventário)

	float shoot();   // Gasta uma bala e devolve o dano do tiro (0 se não houver munição)
	int reload(int amount);   // Coloca até "amount" balas no carregador, sem passar da capacidade. Devolve quantas entraram
	bool hasAmmo() const;   // Tem bala no carregador?

	std::string getName() const { return name; }
	AmmoType getAmmoType() const { return ammoType; }
	float getDamage() const { return damage; }
	int getRange() const { return range; }
	float getFireInterval() const { return fireInterval; }
	int getAmmo() const { return ammo; }
	int getMaxAmmo() const { return maxAmmo; }

};

#endif
