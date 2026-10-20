#include <iostream>

#include "Game.h"

Game::Game() : player(Position()), running(false)
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

	// Entra no loop do jogo e só volta quando o jogador sair
	running = true;
	gameLoop();
}

// Repete até o jogador sair: lê a tecla, atualiza o jogo e redesenha
void Game::gameLoop()
{
	while (running)
	{
		processInput();
		update();
	}

	std::cout << std::endl;
}

// Lê uma tecla do jogador e repassa para a função certa.
// w, a, s, d: o jogador anda. q: sai do jogo. Qualquer outra tecla é ignorada.
void Game::processInput()
{
	char tecla;

	// Apaga a linha (o que foi digitado na jogada anterior) e pede a tecla
	std::cout << "\033[2K" << "Mover (w/a/s/d + Enter), q para sair: " << std::flush;

	if (!(std::cin >> tecla))
	{
		running = false;
		return;
	}

	switch (tecla)
	{
		case 'w': case 'W':
		case 'a': case 'A':
		case 's': case 'S':
		case 'd': case 'D':
			// Os resultados do movimento ainda não são usados
			player.move(tecla, currentSection);
			break;
		case 'q': case 'Q':
			running = false;
			break;
		default:
			break;
	}
}

// Redesenha na tela só o que mudou
void Game::update()
{
	currentSection.update(player);
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
