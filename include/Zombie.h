#ifndef ZOMBIE_H
#define ZOMBIE_H

#include "Character.h"
#include "Position.h"
#include "Player.h"

// Inimigo que fica em uma sala, persegue e ataca o jogador.
class Zombie : public Character
{
private:
	float damage;
	Position startPos;   // Posição inicial, para onde volta quando a sala é recarregada

public:
	Zombie();   // Zumbi com vida e dano provisórios
	Zombie(Position pos);   // Zumbi que nasce na posição dada

	void update(Player target);   // Ainda não implementada
	void attack(Player target);   // Ainda não implementada
	void resetPosition();   // Ainda não implementada

};

#endif
