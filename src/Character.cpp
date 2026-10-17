#include "Character.h"
#include "MapSection.h"

Character::Character(Position pos) : pos(pos) {}

// Tenta andar uma célula na direção dada (w, a, s, d).
// MOVED: andou. BLOCKED: parede, borda do mapa ou direção inválida (não andou).
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

	// Piso: anda
	pos = target;
	return MoveResult::MOVED;
}

void Character::takeDamage(float amount)
{
}

bool Character::isAlive()
{
	return false;
}
