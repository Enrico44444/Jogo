#include "Character.h"
#include "MapSection.h"

// Personagem com a vida cheia (100) na posição dada
Character::Character(Position pos) : life(100), maxLife(100), pos(pos) {}

// Tenta andar uma célula na direção dada (w, a, s, d).
// MOVED: andou. BLOCKED: parede, borda do mapa, zumbi à frente ou direção inválida (não andou).
// DOOR: há uma porta à frente; o personagem não anda e quem chamou decide o que fazer.
MoveResult Character::move(char direction, MapSection& section)
{
	// Célula para qual o character quer se movimentar
	Position target = pos.nextPos(direction);

	// Direção inválida: neste caso nextPos devolve a própria posição
	if (target.equals(pos))
	{
		return MoveResult::BLOCKED;
	}

	// Descobre o que há na célula à frente
	Tile* tile = section.getTileAt(target);
	if (tile == nullptr)
	{
		return MoveResult::BLOCKED;
	}

	// Porta: não anda, só avisa. dynamic_cast devolve nullptr se o tile não for uma Door
	if (dynamic_cast<Door*>(tile) != nullptr)
	{
		return MoveResult::DOOR;
	}

	// Parede: não anda
	if (!tile->isWalkable())
	{
		return MoveResult::BLOCKED;
	}

	// Zumbi na célula: não anda
	// Só os zumbis ficam registrados na sala: o zumbi confere a posição do jogador por conta própria
	if (section.getZombieAt(target) != nullptr)
	{
		return MoveResult::BLOCKED;
	}

	// Piso: anda
	pos = target;
	return MoveResult::MOVED;
}

// Tira vida, sem deixar ficar negativa. Dano negativo é ignorado
void Character::takeDamage(float amount)
{
	if (amount <= 0)
	{
		return;
	}

	life -= amount;
	if (life < 0)
	{
		life = 0;
	}
}

// Está vivo enquanto a vida for maior que zero
bool Character::isAlive() const
{
	return life > 0;
}
