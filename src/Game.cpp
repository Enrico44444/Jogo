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
	render();   // Mostra o inventário (ainda vazio)

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
		render();
	}

	// Deixa o cursor abaixo do inventário, para o terminal não escrever por cima da tela do jogo
	int ultimaLinha = currentSection.getMapY() + 3 + static_cast<int>(player.getInventory().getItems().size());
	std::cout << "\033[" << ultimaLinha + 1 << ";1H" << std::endl;
}

// Lê uma tecla do jogador e repassa para a função certa.
// w, a, s, d: o jogador anda. q: sai do jogo. Qualquer outra tecla é ignorada.
void Game::processInput()
{
	char tecla;

	// A mensagem da jogada anterior sai de cena (o render a apaga da tela)
	message.clear();

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

			// Andou: se há um item no chão ali, tenta pegar
			if (resultado == MoveResult::MOVED)
			{
				pickUpItemHere();
			}
			// Há uma porta à frente: acha a porta, pergunta e, se o jogador quiser, troca de sala
			else if (resultado == MoveResult::DOOR)
			{
				Position posPorta = player.getPos().nextPos(tecla);
				Door* door = currentSection.getDoorAt(posPorta);
				if (door != nullptr && unlockDoor(*door, posPorta) && askEnterDoor(*door))
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

// Mostra abaixo do mapa a mensagem e o inventário.
// Linhas (contando a partir do mapa, que tem mapY linhas): mapY + 1 = aviso das teclas,
// mapY + 2 = mensagem (ou a pergunta da porta), mapY + 3 em diante = inventário
void Game::render()
{
	int linhaMensagem = currentSection.getMapY() + 2;

	// Mensagem: apaga a linha e escreve a atual (fica em branco quando não há nenhuma)
	std::cout << "\033[" << linhaMensagem << ";1H\033[2K" << message;

	// Inventário: apaga da linha seguinte até o fim da tela e escreve de novo, um item por linha
	const Inventory& inventory = player.getInventory();
	int linha = linhaMensagem + 1;
	std::cout << "\033[" << linha << ";1H\033[J";
	std::cout << "Inventario (" << inventory.getItems().size() << "/" << inventory.getMaxSize() << "):";
	for (const std::unique_ptr<Item>& item : inventory.getItems())
	{
		linha++;
		std::cout << "\033[" << linha << ";1H  " << item->getName() << " x" << item->getAmount();
	}

	// Devolve o cursor para a linha do aviso das teclas
	std::cout << "\033[" << linhaMensagem - 1 << ";1H" << std::flush;
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

// Se há um item no chão onde o jogador está, tenta guardá-lo no inventário
void Game::pickUpItemHere()
{
	Position pos = player.getPos();
	Item* item = currentSection.getItemAt(pos);
	if (item == nullptr)
	{
		return;
	}

	// Inventário cheio (e sem pilha desse item): o item continua no chão
	if (!player.canPickUp(*item))
	{
		message = "Inventario cheio: nao deu para pegar " + item->getName();
		return;
	}

	message = "Pegou: " + item->getName();
	player.pickUpItem(currentSection.takeItemAt(pos));

	// Apaga o item do arquivo da sala, para ele não voltar quando a sala for carregada de novo
	if (!currentSection.removeItemFromFile(pos))
	{
		message += " (aviso: arquivo nao atualizado)";
	}
}

// Garante que a porta pode ser atravessada. Devolve true se ela está (ou ficou) destrancada.
// Se estiver trancada, usa a chave do inventário: a chave é consumida e a porta fica destrancada
// também no arquivo da sala. Sem a chave, mostra a mensagem e devolve false.
bool Game::unlockDoor(Door& door, Position pos)
{
	if (!door.isLocked())
	{
		return true;
	}

	// Verifica se 0 jogador tem a chave que essa porta exige
	const KeyItem* key = player.findKey(door.getRequiredKey());
	if (key == nullptr)
	{
		message = "Porta trancada: precisa da chave " + door.getRequiredKey();
		return false;
	}

	// Destranca com a chave. A mensagem é montada antes de consumir a chave (o item some do inventário)
	door.unlock(*key);
	message = "Usou a chave " + door.getRequiredKey() + ": porta destrancada";
	player.useKey(door.getRequiredKey());

	// Apaga a chave da linha DOOR do arquivo, para a porta continuar aberta quando a sala for recarregada
	if (!currentSection.unlockDoorInFile(pos))
	{
		message += " (aviso: arquivo nao atualizado)";
	}
	return true;
}
