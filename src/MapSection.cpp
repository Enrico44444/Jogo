#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <utility>
#include "MapSection.h"
#include "Floor.h"
#include "Wall.h"
#include "HealItem.h"
#include "AmmoItem.h"
#include "KeyItem.h"

bool MapSection::loadFromFile(std::string path)
{	
	// Abre o arquivo que está em path
	std::ifstream file(path);
	if (!file.is_open())
	{
		std::cerr << "Erro: nao foi possivel abrir " << path << std::endl;
		return false;
	}

	// Extrai largura e altura do mapa da primeira linha do arquivo
	std::string map_size;
	if (!std::getline(file, map_size))
		{
		std::cerr << "Erro: arquivo vazio" << std::endl;
		return false;
		}

	int w = 0, h = 0;
	std::istringstream ss(map_size);
	if (!(ss >> w >> h))
	{
		std::cerr << "Erro: primeira linha deve ser 'largura altura'" << std::endl;
		return false;
}

	if (w <= 0 || h <= 0 || w > 200 || h > 200)
	{
		std::cerr << "Erro: largura e altura devem estar entre 1 e 200" << std::endl;
		return false;
	}
	int lineNo = 1;   // Número da linha lida, usado nas mensagens de erro

	// Tudo é armazenado em variáveis LOCAIS e só é copiado para a sala no fim.
	// Assim, um erro no meio do arquivo não deixa a sala pela metade

	std::vector<std::string> grid;                  // Cópia do mapa, para poder conferir as linhas de objetos depois
	std::vector<std::unique_ptr<Tile>> newTiles;    // Tile (x, y) fica em newTiles[y * w + x], usando ponteiro para a as informacoes das subclasses Door, Floor e Wall não serem cortadas 
	std::vector<Door*> newDoors;                    // Atalho para as portas (quem tem a posse é newTiles)
	std::vector<Zombie> newZombies;
	std::vector<std::unique_ptr<Item>> newItems;
	Position newStart;                              // Onde o jogador começa nesta sala

	// Percorrendo as h linhas, cada uma com w caracteres, que representam a grade do mapa
	for (int i = 0; i < h; i++){                    // i = linha (y)
		std::string line;
		if (!std::getline(file, line))
		{
			std::cerr << "Erro: faltam linhas na grade do mapa" << std::endl;
			return false;
		}
		lineNo++;
		// A linha precisa ter exatamente w caracteres ou o mapa está mal formado
		if (static_cast<int>(line.size()) != w) {
			std::cerr << "Erro: linha " << lineNo << ": tem " << line.size()
				<< " caracteres, mas a largura e " << w << std::endl;
			return false;
		}

		for (int j = 0; j < w; j++){                // j = coluna (x)
			char c = line[j];

			// Position recebe (x, y), ou seja, (coluna, linha) = (j, i)
			switch (c) {
				case '#':
					newTiles.push_back(std::unique_ptr<Tile>(new Wall(Position(j, i))));
					break;
				case '.':
				case 'H':
				case 'A':
				case 'K':
					// H, A e K ficam sobre um piso; o item em si vem das linhas abaixo da grade
					newTiles.push_back(std::unique_ptr<Tile>(new Floor(Position(j, i))));
					break;
				case 'D':
				{
					// A porta vai para a lista de tiles (que é a dona) e também para a lista de portas
					Door* door = new Door(Position(j, i));
					newTiles.push_back(std::unique_ptr<Tile>(door));
					newDoors.push_back(door);
					break;
				}
				default:
					std::cerr << "Erro: linha " << lineNo << ": simbolo invalido '" << c
						<< "' na coluna " << j << std::endl;
					return false;
			}
		}
		grid.push_back(line);                       // Guarda a linha para a segunda parte
}

	// Linhas que descrevem os objetos da sala (jogador, zumbis, portas e itens) começam depois do mapa
	// tratado[y * w + x] diz se a célula já recebeu a sua linha (porta ou item)
	// Serve para recusar repetições e, no fim, achar símbolos sem linha correspondente
	std::vector<bool> tratado(w * h, false);
	std::string line;

	// Lê o resto do arquivo, uma linha por vez
	while (std::getline(file, line))
	{
		lineNo++;

		// Divide a linha em palavras e números, a primeira palavra é a tag
		std::istringstream ls(line);
		std::string tag;
		// Linha em branco (sem tag) ou comentário (começa com //) é ignorada
		if (!(ls >> tag) || tag.compare(0, 2, "//") == 0)
		{
			continue;
		}

		// Tag desconhecida: provavelmente erro de digitação no arquivo
		if (tag != "PLAYER" && tag != "ZOMBIE" && tag != "DOOR"
			&& tag != "HEAL" && tag != "AMMO" && tag != "KEY")
		{
			std::cerr << "Erro: linha " << lineNo << ": tag desconhecida " << tag << std::endl;
			return false;
		}

		// Toda tag é seguida de "x y" (coluna e linha, a partir de 0)
		int x = 0, y = 0;
		if (!(ls >> x >> y))
		{
			std::cerr << "Erro: linha " << lineNo << ": esperado 'x y' depois de " << tag << std::endl;
			return false;
		}
		// Fora do mapa: precisa ser conferido antes de usar grid[y][x]
		if (x < 0 || x >= w || y < 0 || y >= h)
		{
			std::cerr << "Erro: linha " << lineNo << ": posicao fora do mapa" << std::endl;
			return false;
		}

		Position p(x, y);
		char celula = grid[y][x];     // Símbolo que a grade tem nessa posição (linha y, coluna x)
		int indice = y * w + x;       // Posição dessa célula nas listas

		if (tag == "PLAYER" || tag == "ZOMBIE")
		{
			// Jogador e zumbi só podem começar sobre um piso
			if (celula != '.' && celula != 'H' && celula != 'A' && celula != 'K')
			{
				std::cerr << "Erro: linha " << lineNo << ": " << tag << " precisa estar sobre um piso" << std::endl;
				return false;
			}
			if (tag == "PLAYER")
			{
				newStart = p;                           // Posição inicial do jogador
			}
			else
			{
				newZombies.push_back(Zombie(p));        // A posição também vira o startPos do zumbi
			}
		}
		else if (tag == "DOOR")
		{
			// A grade precisa ter um 'D' ali, e a porta não pode ser definida duas vezes
			if (celula != 'D')
			{
				std::cerr << "Erro: linha " << lineNo << ": nao ha 'D' na grade nessa posicao" << std::endl;
				return false;
			}
			if (tratado[indice])
			{
				std::cerr << "Erro: linha " << lineNo << ": essa porta ja foi definida" << std::endl;
				return false;
			}

			// arquivo = sala de destino; (tx, ty) = onde o jogador aparece lá; chave é opcional
			std::string arquivo, chave;
			int tx = 0, ty = 0;
			if (!(ls >> arquivo >> tx >> ty))
			{
				std::cerr << "Erro: linha " << lineNo << ": esperado 'DOOR x y arquivo tx ty [chave]'" << std::endl;
				return false;
			}
			ls >> chave;   // Se não houver chave, a leitura falha em silêncio e "chave" continua vazia (nem todas as portas estão trancadas)

			// O tile dessa posição é uma Door (garantido pelo 'D' da grade): preenche destino e chave
			Door* door = static_cast<Door*>(newTiles[indice].get());
			door->configure(arquivo, Position(tx, ty), chave);
			tratado[indice] = true;
		}
		else
		{
			// HEAL, AMMO e KEY: cada um precisa do seu símbolo na grade (H, A ou K)
			char esperado = (tag == "HEAL") ? 'H' : (tag == "AMMO") ? 'A' : 'K';
			if (celula != esperado)
			{
				std::cerr << "Erro: linha " << lineNo << ": nao ha '" << esperado
					<< "' na grade nessa posicao" << std::endl;
				return false;
			}
			// Não pode haver dois itens na mesma célula
			if (tratado[indice])
			{
				std::cerr << "Erro: linha " << lineNo << ": ja existe um item nessa posicao" << std::endl;
				return false;
			}

			if (tag == "HEAL")
			{
				// HEAL x y quantidade: quanta vida a erva recupera
				float quantidade = 0;
				if (!(ls >> quantidade))
				{
					std::cerr << "Erro: linha " << lineNo << ": esperado 'HEAL x y quantidade'" << std::endl;
					return false;
				}
				newItems.push_back(std::unique_ptr<Item>(new HealItem(p, quantidade)));
			}
			else if (tag == "AMMO")
			{
				// AMMO x y tipo quantidade: o tipo vem como texto e vira o enum AmmoType
				std::string nomeTipo;
				int quantidade = 0;
				if (!(ls >> nomeTipo >> quantidade))
				{
					std::cerr << "Erro: linha " << lineNo << ": esperado 'AMMO x y PISTOL|SHOTGUN|RIFLE quantidade'" << std::endl;
					return false;
				}

				AmmoType tipo = AmmoType::PISTOL;
				if (nomeTipo == "PISTOL")
				{
					tipo = AmmoType::PISTOL;
				}
				else if (nomeTipo == "SHOTGUN")
				{
					tipo = AmmoType::SHOTGUN;
				}
				else if (nomeTipo == "RIFLE")
				{
					tipo = AmmoType::RIFLE;
				}
				else
				{
					std::cerr << "Erro: linha " << lineNo << ": tipo de municao desconhecido " << nomeTipo << std::endl;
					return false;
				}
				newItems.push_back(std::unique_ptr<Item>(new AmmoItem(p, tipo, quantidade)));
			}
			else
			{
				// KEY x y id: o mesmo id aparece na linha DOOR da porta que essa chave abre
				std::string id;
				if (!(ls >> id))
				{
					std::cerr << "Erro: linha " << lineNo << ": esperado 'KEY x y id'" << std::endl;
					return false;
				}
				newItems.push_back(std::unique_ptr<Item>(new KeyItem(p, id)));
			}
			tratado[indice] = true;     // Marca a célula como ocupada por um item
		}
	}

	// CONFERÊNCIA FINAL: todo D, H, A e K da grade precisa ter sua linha
	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{
			char c = grid[y][x];
			if ((c == 'D' || c == 'H' || c == 'A' || c == 'K') && !tratado[y * w + x])
			{
				std::cerr << "Erro: '" << c << "' em (" << x << ", " << y << ") sem a linha correspondente" << std::endl;
				return false;
			}
		}
	}

	// Caso não tenha ocorrido nenhum erro: agora a sala é atualizada
	mapX = w;
	mapY = h;
	tiles = std::move(newTiles);        // Move transfere o conteúdo sem copiar
	doors = std::move(newDoors);        // (os ponteiros continuam válidos: os objetos não mudam de lugar)
	zombies = std::move(newZombies);
	items = std::move(newItems);
	playerStart = newStart;
	lastFrame.clear();                  // a tela mostra outra sala: o próximo update desenha tudo de novo
	return true;
}

// Monta o desenho do mapa:
// Cada "camada" sobrescreve a anterior, então a última a ser escrita aparece por cima:
// Ordem: tiles < itens < zumbis < jogador
std::vector<char> MapSection::buildFrame(Position playerPos) const
{
	std::vector<char> frame(mapX * mapY, ' ');

	// Camada 1: terreno (piso, parede, porta)
	for (std::size_t i = 0; i < tiles.size(); i++)
	{
		frame[i] = tiles[i]->symbol();
	}

	// Camada 2: itens
	for (const std::unique_ptr<Item>& item : items)
	{
		Position p = item->getPos();
		frame[p.getY() * mapX + p.getX()] = item->symbol();
	}

	// Camada 3: zumbis
	for (const Zombie& zombie : zombies)
	{
		Position p = zombie.getPos();
		frame[p.getY() * mapX + p.getX()] = 'Z';
	}

	// Camada 4: jogador
	if (playerPos.getX() >= 0 && playerPos.getX() < mapX && playerPos.getY() >= 0 && playerPos.getY() < mapY)
	{
		frame[playerPos.getY() * mapX + playerPos.getX()] = 'P';
	}

	return frame;
}

// Limpa a tela e imprime o mapa inteiro. Guarda o desenho em lastFrame.
// Depois da limpeza o mapa começa na linha 1, coluna 1 da tela, e é isso que permite ao update
// escrever em uma célula específica sem imprimir o mapa de novo.
void MapSection::draw(Position playerPos)
{
	if (tiles.empty())
	{
		return;
	}

	lastFrame = buildFrame(playerPos);

	std::cout << "\033[2J\033[H";   // Escape code para limpar a tela e levar o cursor para o canto superior esquerdo
	for (int y = 0; y < mapY; y++)
	{
		for (int x = 0; x < mapX; x++)
		{
			std::cout << lastFrame[y * mapX + x] << ' ';   // Cada célula ocupa 2 colunas: o símbolo e um espaço
		}
		std::cout << '\n';
	}
	std::cout << std::flush;
}

// Imprime o mapa inteiro, com o jogador na posição inicial da sala
void MapSection::render()
{
	draw(playerStart);
}

// Atualiza o desenho depois de uma mudança.
// Monta o desenho novo, compara com o último e reescreve só as células que mudaram.
void MapSection::update(const Player& player)
{
	// Nada desenhado ainda, ou a sala foi recarregada: desenha tudo
	if (lastFrame.size() != tiles.size())
	{
		draw(player.getPos());
		return;
	}

	std::vector<char> frame = buildFrame(player.getPos());

	for (int y = 0; y < mapY; y++)
	{
		for (int x = 0; x < mapX; x++)
		{
			int i = y * mapX + x;
			if (frame[i] != lastFrame[i])
			{
				// \033[linha;colunaH move o cursor para a célula que foi alterada.
				// A tela conta a partir de 1, e cada célula ocupa 2 colunas
				std::cout << "\033[" << y + 1 << ";" << 2 * x + 1 << "H" << frame[i];
			}
		}
	}
	lastFrame = frame;

	// Deixa o cursor logo abaixo do mapa, onde o resto do jogo pode escrever
	std::cout << "\033[" << mapY + 1 << ";1H" << std::flush;
}

int MapSection::getMapX() const {
	return mapX;
}

int MapSection::getMapY() const {
	return mapY;
}

void MapSection::resetZombies()
{
}

// Devolve o tile da posição dada
Tile* MapSection::getTileAt(Position pos)
{
	int x = pos.getX();
	int y = pos.getY();
	if (x < 0 || x >= mapX || y < 0 || y >= mapY)
	{
		return nullptr;
	}
	return tiles[y * mapX + x].get();
}

bool MapSection::isWalkable(Position pos)
{
	return false;
}

// Devolve a porta da posição dada
Door* MapSection::getDoorAt(Position pos)
{
	// dynamic_cast devolve nullptr se o tile não for uma Door (e também se getTileAt devolver nullptr)
	return dynamic_cast<Door*>(getTileAt(pos));
}

Zombie MapSection::getZombieAt(Position pos)
{
	return Zombie();
}

Item MapSection::getItemAt(Position pos)
{
	return Item();
}

// Sem argumento, cria uma sala vazia; com um caminho, já carrega a sala desse arquivo
MapSection::MapSection(std::string filePath) : mapX(0), mapY(0)
{
	if (!filePath.empty())
	{
		loadFromFile(filePath);
	}
}
