#ifndef WEAPON_H
#define WEAPON_H

#include <string>

class Weapon
{
private:
	std::string name;
	float damage;
	int ammo;
	int maxAmmo;

public:
	float shoot();
	void reload(int amount);
	bool hasAmmo();

};

#endif
