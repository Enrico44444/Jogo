#include "Weapon.h"

// Pistola com o carregador cheio
Weapon::Weapon() : Weapon(AmmoType::PISTOL) {}

// Arma do tipo dado com o carregador cheio. Cada tipo tem seus próprios números:
// a pistola é equilibrada, a escopeta bate forte de perto e o rifle alcança longe, mas tem poucas balas
Weapon::Weapon(AmmoType type) : ammoType(type)
{
	if (type == AmmoType::SHOTGUN)
	{
		name = "Escopeta";
		damage = 70;
		range = 3;
		fireInterval = 0.9f;
		maxAmmo = 6;
	}
	else if (type == AmmoType::RIFLE)
	{
		name = "Rifle";
		damage = 50;
		range = 8;
		fireInterval = 0.6f;
		maxAmmo = 5;
	}
	else
	{
		name = "Pistola";
		damage = 25;
		range = 5;
		fireInterval = 0.3f;
		maxAmmo = 12;
	}
	ammo = maxAmmo;
}

// Arma do tipo dado com "ammo" balas no carregador (nunca menos que 0 nem mais que a capacidade)
Weapon::Weapon(AmmoType type, int ammo) : Weapon(type)
{
	if (ammo < 0)
	{
		ammo = 0;
	}
	if (ammo > maxAmmo)
	{
		ammo = maxAmmo;
	}
	this->ammo = ammo;
}

// Atira: gasta uma bala e devolve o dano do tiro. Sem munição, não atira (devolve 0)
float Weapon::shoot()
{
	if (!hasAmmo())
	{
		return 0;
	}

	ammo--;
	return damage;
}

// Coloca balas no carregador, sem passar da capacidade. Devolve quantas balas entraram
int Weapon::reload(int amount)
{
	if (amount <= 0)
	{
		return 0;
	}

	int espaco = maxAmmo - ammo;
	int entrou = (amount < espaco) ? amount : espaco;
	ammo += entrou;
	return entrou;
}

// Tem bala no carregador?
bool Weapon::hasAmmo() const
{
	return ammo > 0;
}
