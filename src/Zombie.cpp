#include <cstdlib>

#include "Zombie.h"
#include "MapSection.h"

// Alcance da visão do zumbi:
static const int VISION_RANGE = 6;

// Tempo entre dois passos do zumbi (em segundos):
static const float ZOMBIE_MOVE_INTERVAL = 1.0f;

// Tempo entre dois ataques do zumbi (em segundos):
static const float ZOMBIE_ATTACK_INTERVAL = 1.0f;

// Quando o zumbi encosta no jogador, o primeiro ataque demora este tempo:
static const float ZOMBIE_ATTACK_WARMUP = 0.5f;

// Zumbi padrão. Vida e dano provisórios
Zombie::Zombie() : Character(Position()), damage(10), startPos(), moveTimer(ZOMBIE_MOVE_INTERVAL), attackTimer(ZOMBIE_ATTACK_WARMUP), adjacent(false)
{
	life = 100;
}

// Zumbi na posição dada, essa posição também é a inicial (startPos)
Zombie::Zombie(Position pos) : Character(pos), damage(10), startPos(pos), moveTimer(ZOMBIE_MOVE_INTERVAL), attackTimer(ZOMBIE_ATTACK_WARMUP), adjacent(false)
{
	life = 100;
}

// Zumbi vivo: percebe o jogador a até VISION_RANGE células. Ao lado dele, ataca (a cada ZOMBIE_ATTACK_INTERVAL),
// senão, anda uma célula em direção a ele (a cada ZOMBIE_MOVE_INTERVAL).
// Devolve true se algo mudou na tela (o zumbi andou ou atacou)
bool Zombie::update(Player& target, MapSection& section, float dt)
{
	if (!isAlive())
	{
		return false;
	}

	// Os temporizadores contam o tempo todo, mesmo com o jogador longe
	if (moveTimer > 0)
	{
		moveTimer -= dt;
	}
	if (attackTimer > 0)
	{
		attackTimer -= dt;
	}

	// dx e dy dizem para que lado o jogador está, a distância soma os dois
	int dx = target.getPos().getX() - pos.getX();
	int dy = target.getPos().getY() - pos.getY();
	int distancia = std::abs(dx) + std::abs(dy);

	// Ao lado do jogador (acima, abaixo, à esquerda ou à direita).
	// Na primeira vez que encosta, o ataque espera ZOMBIE_ATTACK_WARMUP
	bool aoLado = (distancia == 1);
	if (aoLado && !adjacent && attackTimer < ZOMBIE_ATTACK_WARMUP)
	{
		attackTimer = ZOMBIE_ATTACK_WARMUP;
	}
	adjacent = aoLado;

	// Longe demais: o zumbi não percebe o jogador
	if (distancia > VISION_RANGE)
	{
		return false;
	}

	// Ao lado: ataca quando o tempo de espera acabar
	if (aoLado)
	{
		if (attackTimer > 0)
		{
			return false;
		}

		attack(target);
		attackTimer = ZOMBIE_ATTACK_INTERVAL;
		return true;
	}

	// Ainda não é hora de dar o próximo passo
	if (moveTimer > 0)
	{
		return false;
	}

	// O tempo recomeça mesmo se o passo for bloqueado (assim ele não tenta de novo a cada quadro)
	moveTimer = ZOMBIE_MOVE_INTERVAL;

	// Há até duas direções que diminuem a distância: a do eixo em que o jogador está mais longe vem primeiro
	char horizontal = (dx > 0) ? 'd' : 'a';
	char vertical = (dy > 0) ? 's' : 'w';
	char opcoes[2];
	int quantas = 0;
	if (std::abs(dx) >= std::abs(dy))
	{
		if (dx != 0)
		{
			opcoes[quantas++] = horizontal;
		}
		if (dy != 0)
		{
			opcoes[quantas++] = vertical;
		}
	}
	else
	{
		if (dy != 0)
		{
			opcoes[quantas++] = vertical;
		}
		if (dx != 0)
		{
			opcoes[quantas++] = horizontal;
		}
	}

	for (int i = 0; i < quantas; i++)
	{
		// Não pisa no jogador nem em outro zumbi
		Position destino = pos.nextPos(opcoes[i]);
		if (destino.equals(target.getPos()) || section.getZombieAt(destino) != nullptr)
		{
			continue;
		}

		// O move só devolve MOVED se a célula for piso livre: parede e porta bloqueiam o zumbi
		if (move(opcoes[i], section) == MoveResult::MOVED)
		{
			return true;
		}
	}
	return false;
}

// Ataca o jogador: tira o dano do zumbi da vida dele
void Zombie::attack(Player& target)
{
	target.takeDamage(damage);
}

// Volta para a posição inicial, e os temporizadores recomeçam
void Zombie::resetPosition()
{
	pos = startPos;
	moveTimer = ZOMBIE_MOVE_INTERVAL;
	attackTimer = ZOMBIE_ATTACK_WARMUP;
	adjacent = false;
}
