#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

#include "Game.h"
#include "Input.h"

// Intervalo mínimo entre dois passos do jogador (em segundos). Segurar a tecla gera muitos eventos por segundo,
// e sem esse limite o jogador atravessaria a sala disparado
static const float MOVE_INTERVAL = 0.12f;

// Quanto tempo uma mensagem fica na tela (em segundos)
static const float MESSAGE_TIME = 3.0f;

// Duração de cada quadro do jogo (em milissegundos): 33 ms são cerca de 30 quadros por segundo
static const int FRAME_MS = 33;

Game::Game() : player(Position()), running(false), messageTimer(0), moveCooldown(0), needsRedraw(true)
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

	// Teclado sem Enter e sem travar o jogo; o cursor some. O Input restaura tudo ao sair
	Input::init();

	// Entra no loop do jogo e só volta quando o jogador sair
	running = true;
	gameLoop();

	Input::restore();
}

// Repete cerca de 30 vezes por segundo até o jogador sair: lê as teclas, atualiza o jogo com o tempo
// que passou e redesenha
void Game::gameLoop()
{
	auto anterior = std::chrono::steady_clock::now();
	while (running)
	{
		auto inicio = std::chrono::steady_clock::now();
		float dt = std::chrono::duration<float>(inicio - anterior).count();   // segundos desde o quadro anterior
		anterior = inicio;

		processInput();
		update(dt);
		render();

		// Dorme o que sobrar do quadro, para não gastar a CPU à toa
		auto gasto = std::chrono::steady_clock::now() - inicio;
		auto quadro = std::chrono::milliseconds(FRAME_MS);
		if (gasto < quadro)
		{
			std::this_thread::sleep_for(quadro - gasto);
		}
	}

	// Deixa o cursor abaixo do inventário, para o terminal não escrever por cima da tela do jogo
	int ultimaLinha = currentSection.getMapY() + 4 + static_cast<int>(player.getInventory().getItems().size());
	std::cout << "\033[" << ultimaLinha + 1 << ";1H" << std::endl;
}

// Lê todas as teclas que chegaram desde o quadro anterior (sem esperar nenhuma) e trata cada uma
void Game::processInput()
{
	char tecla;
	Input::Result resultado;
	while ((resultado = Input::read(tecla)) == Input::KEY)
	{
		handleKey(tecla);
	}

	// A entrada foi fechada, encerra o jogo
	if (resultado == Input::END_OF_INPUT)
	{
		running = false;
	}
}

// Trata uma tecla e repassa para a função certa.
// w, a, s, d (ou as setas): o jogador anda, no máximo um passo a cada MOVE_INTERVAL.
// 1 a 8: usa o item de mesmo número do inventário. q: sai do jogo. Qualquer outra tecla é ignorada.
void Game::handleKey(char key)
{
	switch (key)
	{
		case 'w': case 'W':
		case 'a': case 'A':
		case 's': case 'S':
		case 'd': case 'D':
		{
			// Ainda no intervalo do passo anterior: ignora a tecla (segurar a tecla não faz o jogador disparar)
			if (moveCooldown > 0)
			{
				break;
			}

			MoveResult resultado = player.move(key, currentSection);

			// Andou: começa o intervalo até o próximo passo e, se há um item no chão ali, tenta pegar
			if (resultado == MoveResult::MOVED)
			{
				moveCooldown = MOVE_INTERVAL;
				needsRedraw = true;
				pickUpItemHere();
			}
			// Há uma porta à frente: acha a porta, pergunta e, se o jogador quiser, troca de sala
			else if (resultado == MoveResult::DOOR)
			{
				Position posPorta = player.getPos().nextPos(key);
				Door* door = currentSection.getDoorAt(posPorta);
				if (door != nullptr && unlockDoor(*door, posPorta) && askEnterDoor(*door))
				{
					changeSection(*door);
				}
				needsRedraw = true;   // a pergunta mexeu na tela e a sala pode ter mudado
			}
			break;
		}
		case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8':
			useInventoryItem(key - '0');
			break;
		case 'q': case 'Q':
			running = false;
			break;
		default:
			break;
	}
}

// Avança o tempo (o intervalo entre passos e a vida da mensagem). Depois, se algo mudou, redesenha na tela só o que mudou
void Game::update(float dt)
{
	if (moveCooldown > 0)
	{
		moveCooldown -= dt;
		if (moveCooldown < 0)
		{
			moveCooldown = 0;
		}
	}

	// A mensagem sai de cena quando o tempo dela acaba
	if (!message.empty())
	{
		messageTimer -= dt;
		if (messageTimer <= 0)
		{
			message.clear();
			needsRedraw = true;
		}
	}

	// O mapa só é redesenhado quando algo mudou (o MapSection::update escreve só as células diferentes)
	if (needsRedraw)
	{
		currentSection.update(player);
	}
}

// Mostra abaixo do mapa a mensagem, a vida e o inventário.
// Linhas (contando a partir do mapa, que tem mapY linhas): mapY + 1 = ajuda das teclas,
// mapY + 2 = mensagem (ou a pergunta da porta), mapY + 3 = vida, mapY + 4 em diante = inventário
void Game::render()
{
	// Só redesenha quando algo mudou (a 30 quadros por segundo, reescrever tudo sempre faria a tela piscar)
	if (!needsRedraw)
	{
		return;
	}
	needsRedraw = false;

	int linhaMensagem = currentSection.getMapY() + 2;

	// Ajuda das teclas (linha mapY + 1)
	std::cout << "\033[" << linhaMensagem - 1 << ";1H\033[2K" << "Andar: setas ou w/a/s/d  |  Usar item: 1 a 8  |  Sair: q";

	// Mensagem: apaga a linha e escreve a atual (fica em branco quando não há nenhuma)
	std::cout << "\033[" << linhaMensagem << ";1H\033[2K" << message;

	// Vida: apaga a linha e escreve a vida atual / máxima
	std::cout << "\033[" << linhaMensagem + 1 << ";1H\033[2K" << "Vida: " << static_cast<int>(player.getLife()) << "/" << static_cast<int>(player.getMaxLife());

	// Inventário: apaga da linha seguinte até o fim da tela e escreve de novo, um item por linha.
	// Os itens são numerados: é o número da tecla que usa o item
	const Inventory& inventory = player.getInventory();
	int linha = linhaMensagem + 2;
	std::cout << "\033[" << linha << ";1H\033[J";
	std::cout << "Inventário (" << inventory.getItems().size() << "/" << inventory.getMaxSize() << "):";
	int numero = 0;
	for (const std::unique_ptr<Item>& item : inventory.getItems())
	{
		numero++;
		linha++;
		std::cout << "\033[" << linha << ";1H  " << numero << ") " << item->getName() << " x" << item->getAmount();
	}

	// Devolve o cursor para a linha da ajuda das teclas (ele está escondido, mas o MapSection::update conta com isso)
	std::cout << "\033[" << linhaMensagem - 1 << ";1H" << std::flush;
}

// Pergunta se o jogador quer atravessar a porta. Devolve true se a resposta for sim.
// O jogo fica parado esperando a resposta: só s (sim) ou n (não) valem, as outras teclas são ignoradas
bool Game::askEnterDoor(Door door)
{
	// A pergunta fica sempre na linha logo abaixo da ajuda das teclas (mapY + 1)
	int linha = currentSection.getMapY() + 2;

	// \033[linha;1H leva o cursor para o início dessa linha e \033[2K apaga o que houver nela
	std::cout << "\033[" << linha << ";1H\033[2K" << "Entrar em " << door.getTargetFile() << "? (s/n): " << std::flush;

	bool sim = false;
	while (true)
	{
		char resposta;

		// Fim da entrada, encerra o jogo
		if (Input::wait(resposta) == Input::END_OF_INPUT)
		{
			running = false;
			break;
		}

		if (resposta == 's' || resposta == 'S')
		{
			sim = true;
			break;
		}
		if (resposta == 'n' || resposta == 'N')
		{
			break;
		}
	}

	// Apaga a pergunta, para ela não ficar na tela depois de respondida
	std::cout << "\033[" << linha << ";1H\033[2K" << std::flush;

	return sim;
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
		showMessage("Inventario cheio: nao deu para pegar " + item->getName());
		return;
	}

	std::string texto = "Pegou: " + item->getName();
	player.pickUpItem(currentSection.takeItemAt(pos));

	// Apaga o item do arquivo da sala, para ele não voltar quando a sala for carregada de novo
	if (!currentSection.removeItemFromFile(pos))
	{
		texto += " (aviso: arquivo nao atualizado)";
	}
	showMessage(texto);
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
		showMessage("Porta trancada: precisa da chave " + door.getRequiredKey());
		return false;
	}

	// Destranca com a chave. A mensagem é montada antes de consumir a chave (o item some do inventário)
	door.unlock(*key);
	std::string texto = "Usou a chave " + door.getRequiredKey() + ": porta destrancada";
	player.useKey(door.getRequiredKey());

	// Apaga a chave da linha DOOR do arquivo, para a porta continuar aberta quando a sala for recarregada
	if (!currentSection.unlockDoorInFile(pos))
	{
		texto += " (aviso: arquivo nao atualizado)";
	}
	showMessage(texto);
	return true;
}

// Usa o item de número "numero" da lista do inventário (o mesmo número mostrado na tela).
// A mensagem do resultado vem pronta do próprio item
void Game::useInventoryItem(int numero)
{
	const std::vector<std::unique_ptr<Item>>& itens = player.getInventory().getItems();
	if (numero < 1 || numero > static_cast<int>(itens.size()))
	{
		showMessage("Não existe o item " + std::to_string(numero));
		return;
	}

	// O nome é copiado antes: se o item for gasto por inteiro, ele sai da lista
	std::string nome = itens[numero - 1]->getName();
	std::string resultado;
	player.useItem(nome, resultado);
	showMessage(resultado);
}

// Mostra uma mensagem abaixo do mapa. Ela some sozinha depois de MESSAGE_TIME segundos
void Game::showMessage(const std::string& text)
{
	message = text;
	messageTimer = MESSAGE_TIME;
	needsRedraw = true;
}
