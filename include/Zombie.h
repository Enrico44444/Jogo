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
	float moveTimer;   // Segundos até o zumbi poder dar o próximo passo
	float attackTimer;   // Segundos até o zumbi poder atacar de novo
	bool adjacent;   // Estava ao lado do jogador no quadro anterior?

public:
	Zombie();   // Zumbi com vida e dano provisórios
	Zombie(Position pos);   // Zumbi que nasce na posição dada

	bool update(Player& target, MapSection& section, float dt);   // Ataca o jogador se estiver ao lado dele, senão anda em direção a ele. Devolve true se andou ou atacou
	void attack(Player& target);   // Tira o dano do zumbi da vida do jogador
	Position getStartPos() const { return startPos; }   // Posição inicial (a mesma da linha ZOMBIE do arquivo da sala)
	void resetPosition();   // Volta para a posição inicial

};

#endif
