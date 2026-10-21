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
		{
			MoveResult resultado = player.move(tecla, currentSection);

			// Há uma porta à frente: acha a porta, pergunta e, se o jogador quiser, troca de sala
			if (resultado == MoveResult::DOOR)
			{
				Door* door = currentSection.getDoorAt(player.getPos().nextPos(tecla));
				if (door != nullptr && askEnterDoor(*door))
				{
					changeSection(*door);
				}
			}
			break;
		}
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

// Pergunta se o jogador quer atravessar a porta. Devolve true se a resposta for sim
bool Game::askEnterDoor(Door door)
{
	char resposta;

	// A pergunta fica sempre na linha logo abaixo do aviso das teclas (mapY + 1)
	int linha = currentSection.getMapY() + 2;

	// \033[linha;1H leva o cursor para o início dessa linha e \033[2K apaga o que houver nela
	std::cout << "\033[" << linha << ";1H\033[2K" << "Entrar em " << door.getTargetFile() << "? (s/n): " << std::flush;

	// Fim da entrada, encerra o jogo
	if (!(std::cin >> resposta))
	{
		running = false;
		return false;
	}

	// Apaga a pergunta, para ela não ficar na tela depois de respondida
	std::cout << "\033[" << linha << ";1H\033[2K" << std::flush;

	return resposta == 's' || resposta == 'S';
}

// Troca a sala atual pela sala de destino da porta e coloca o jogador onde a porta indica.
void Game::changeSection(Door door)
{
	// Se o arquivo de destino tiver erro, o loadFromFile devolve false
	// e a sala atual continua exatamente como estava
	if (!currentSection.loadFromFile(door.getTargetFile()))
	{
		return;
	}

	// Só a posição muda
	// Se a posição de chegada da porta não for um piso, usa a posição inicial da nova sala
	Position destino = door.getTargetPos();
	Tile* tile = currentSection.getTileAt(destino);
	if (tile == nullptr || !tile->isWalkable())
	{
		destino = currentSection.getPlayerStart();
	}
	player.setPos(destino);

}
