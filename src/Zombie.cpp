#include "Zombie.h"

// Zumbi padrão. Vida e dano provisórios
Zombie::Zombie() : Character(Position()), damage(10), startPos()
{
	life = 100;
}

// Zumbi na posição dada; essa posição também é a inicial (startPos)
Zombie::Zombie(Position pos) : Character(pos), damage(10), startPos(pos)
{
	life = 100;
}


void Zombie::update(Player& target)
{
}

void Zombie::attack(Player& target)
{
}

void Zombie::resetPosition()
{
}
