#include "Player.h"

Player::Player(Position pos) : Character(pos) {}

float Player::shoot()
{
	return 0;
}

void Player::heal(float amount)
{
}

bool Player::pickUpItem(Item item)
{
	return false;
}
