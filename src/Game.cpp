#include <iostream>

#include "Game.h"

Game::Game() : player(Position())
{
}

void Game::start()
{
	if (!currentSection.loadFromFile("MapSections/map1.txt"))
	{
		return;
	}

	player = Player(currentSection.getPlayerStart());
	currentSection.render();
}

void Game::gameLoop()
{
}

void Game::processInput()
{
}

void Game::update()
{
}

void Game::render()
{
}

bool Game::askEnterDoor(Door door)
{
	return false;
}

void Game::changeSection(Door door)
{
}
