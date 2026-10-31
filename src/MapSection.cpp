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
	currentPath = path;                 // guarda de qual arquivo a sala veio (o removeItemFromFile edita esse arquivo)
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

// Devolve o item do chão na posição dada (sem passar a posse), ou nullptr se não houver
Item* MapSection::getItemAt(Position pos)
{
	for (const std::unique_ptr<Item>& item : items)
	{
		if (item->getPos().equals(pos))
		{
			return item.get();
		}
	}
	return nullptr;
}

// Tira o item da sala e entrega a posse a quem chamou (que normalmente o guarda no inventário).
// Devolve nullptr se não houver item na posição.
std::unique_ptr<Item> MapSection::takeItemAt(Position pos)
{
	for (std::size_t i = 0; i < items.size(); i++)
	{
		if (items[i]->getPos().equals(pos))
		{
			std::unique_ptr<Item> pego = std::move(items[i]);   // a posse sai da lista
			items.erase(items.begin() + i);                     // e a vaga vazia é removida
			return pego;
		}
	}
	return nullptr;
}

// Apaga do arquivo .txt desta sala o item que estava na posição dada:
// o símbolo que era de item vira piso e a linha que descreve o item é removida.
// Assim o item não volta quando a sala for carregada de novo.
// Devolve false se não conseguir.
bool MapSection::removeItemFromFile(Position pos)
{
	// Lê o arquivo inteiro
	std::ifstream in(currentPath);
	if (!in.is_open())
	{
		return false;
	}
	std::stringstream conteudo;
	// rdbuf pega o conteúdo inteiro e coloca em uma linha.
	conteudo << in.rdbuf();
	in.close();

	// Separa em linhas. Se a linha terminava em \n, o \n continua no fim dela
	std::vector<std::string> lines;
	std::string texto = conteudo.str();
	std::size_t inicio = 0;
	while (true)
	{
		std::size_t fim = texto.find('\n', inicio);
		if (fim == std::string::npos) // npos é o que find devolve caso não encontre o caractere inserido no parâmetro
		{
			lines.push_back(texto.substr(inicio));   // última linha (vazia se o arquivo terminava em \n)
			break;
		}
		lines.push_back(texto.substr(inicio, fim - inicio));
		inicio = fim + 1;
	}

	// A primeira linha tem "largura altura"; as h linhas seguintes são a grade
	int w = 0, h = 0;
	std::istringstream tamanho(lines[0]);
	if (!(tamanho >> w >> h))
	{
		return false;
	}

	int x = pos.getX();
	int y = pos.getY();
	if (x < 0 || x >= w || y < 0 || y >= h || static_cast<int>(lines.size()) < h + 1)
	{
		return false;
	}

	// O símbolo da grade nessa posição precisa ser o de um item
	std::string& linhaGrade = lines[1 + y];
	if (static_cast<int>(linhaGrade.size()) < w)
	{
		return false;
	}
	char simbolo = linhaGrade[x];
	if (simbolo != 'H' && simbolo != 'A' && simbolo != 'K')
	{
		return false;
	}
	std::string etiqueta = (simbolo == 'H') ? "HEAL" : (simbolo == 'A') ? "AMMO" : "KEY";

	// Procura, depois da grade, a linha desse item (etiqueta e posição iguais)
	int alvo = -1;
	for (std::size_t i = h + 1; i < lines.size(); i++)
	{
		std::istringstream ls(lines[i]);
		std::string tag;
		int lx = -1, ly = -1;
		if (ls >> tag >> lx >> ly && tag == etiqueta && lx == x && ly == y)
		{
			alvo = static_cast<int>(i);
			break;
		}
	}
	if (alvo == -1)
	{
		return false;
	}

	// Só altera depois de achar tudo: o símbolo vira piso e a linha do item sai
	linhaGrade[x] = '.';
	lines.erase(lines.begin() + alvo);

	// Grava o arquivo de volta
	std::ofstream out(currentPath);
	if (!out.is_open())
	{
		return false;
	}
	for (std::size_t i = 0; i < lines.size(); i++)
	{
		if (i > 0)
		{
			out << '\n';
		}
		out << lines[i];
	}
	// good só retorna true caso não haja erros.
	return out.good();
}

// Deixa a porta dessa posição destrancada no arquivo .txt da sala: apaga a chave da linha DOOR
// ("DOOR x y arquivo tx ty chave" vira "DOOR x y arquivo tx ty").
// Assim a porta continua aberta quando a sala for carregada de novo.
// Devolve false se não conseguir.
bool MapSection::unlockDoorInFile(Position pos)
{
	// Lê o arquivo inteiro
	std::ifstream in(currentPath);
	if (!in.is_open())
	{
		return false;
	}
	std::stringstream conteudo;
	conteudo << in.rdbuf();
	in.close();

	// Separa em linhas
	std::vector<std::string> lines;
	std::string texto = conteudo.str();
	std::size_t inicio = 0;
	while (true)
	{
		std::size_t fim = texto.find('\n', inicio);
		if (fim == std::string::npos)
		{
			lines.push_back(texto.substr(inicio));
			break;
		}
		lines.push_back(texto.substr(inicio, fim - inicio));
		inicio = fim + 1; 
	}

	// A primeira linha tem "largura altura"; as h linhas seguintes são a grade
	int w = 0, h = 0;
	std::istringstream tamanho(lines[0]);
	if (!(tamanho >> w >> h))
	{
		return false;
	}

	int x = pos.getX();
	int y = pos.getY();
	if (x < 0 || x >= w || y < 0 || y >= h || static_cast<int>(lines.size()) < h + 1)
	{
		return false;
	}

	// A grade precisa ter uma porta (D) nessa posição
	if (static_cast<int>(lines[1 + y].size()) < w || lines[1 + y][x] != 'D')
	{
		return false;
	}

	// Procura, depois da grade, a linha DOOR dessa posição
	int alvo = -1;
	for (std::size_t i = h + 1; i < lines.size(); i++)
	{
		std::istringstream ls(lines[i]);
		std::string tag;
		int lx = -1, ly = -1;
		if (ls >> tag >> lx >> ly && tag == "DOOR" && lx == x && ly == y)
		{
			alvo = static_cast<int>(i);
			break;
		}
	}
	if (alvo == -1)
	{
		return false;
	}

	// A linha tem 6 campos fixos (DOOR x y arquivo tx ty) e, se houver, a chave como 7o campo.
	// Procura onde termina o 6o campo: o que vier depois é a chave
	std::string& linha = lines[alvo];
	std::size_t fimCampo = 0;
	for (int campo = 0; campo < 6; campo++)
	{
		std::size_t comeco = linha.find_first_not_of(" \t\r", fimCampo);   // começo do próximo campo
		if (comeco == std::string::npos)
		{
			return false;   // faltam campos: linha malformada
		}
		fimCampo = linha.find_first_of(" \t\r", comeco);                    // fim desse campo
		if (fimCampo == std::string::npos)
		{
			fimCampo = linha.size();
		}
	}

	// Nada depois do 6o campo: a porta já está sem chave, não há o que gravar
	if (linha.find_first_not_of(" \t\r", fimCampo) == std::string::npos)
	{
		return true;
	}

	// Corta a chave e grava o arquivo de volta
	linha.erase(fimCampo);

	std::ofstream out(currentPath);
	if (!out.is_open())
	{
		return false;
	}
	for (std::size_t i = 0; i < lines.size(); i++)
	{
		if (i > 0)
		{
			out << '\n';
		}
		out << lines[i];
	}
	return out.good();
}

// Sem argumento, cria uma sala vazia; com um caminho, já carrega a sala desse arquivo
MapSection::MapSection(std::string filePath) : mapX(0), mapY(0)
{
	if (!filePath.empty())
	{
		loadFromFile(filePath);
	}
}
