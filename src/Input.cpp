#include <cerrno>
#include <chrono>
#include <csignal>
#include <cstdlib>
#include <thread>

#include <sys/select.h>
#include <termios.h>
#include <unistd.h>

#include "Input.h"

namespace // Tudo que está aqui, só existe aqui
{
	bool ativo = false;              // init já foi chamado e restore ainda não
	bool terminalAlterado = false;   // o modo do terminal foi mesmo alterado
	struct termios original;         // configuração do terminal antes do init

	// Escreve direto na saída, sem precisar passar pelo cout
	void escrever(const char* texto, int tamanho)
	{
		// Escreve diretamente no terminal
		// STDOUT_FILENO é um descritor de arquivo, indica para onde o programa deve enviar a saida, nesse caso, é o terminal
		// write(), da unistd.h escreve em um descritor de arquivo
		ssize_t r = write(STDOUT_FILENO, texto, tamanho);
		(void)r; // Indicando que a variável não será mais usada, para que as flags de compilação não "apitem"
	}

	// Desfaz o init: restaura o terminal e mostra o cursor. Pode ser chamada várias vezes
	void restaurar()
	{
		if (!ativo)
		{
			return;
		}
		ativo = false;

		if (terminalAlterado)
		{
			// tcsetattr altera as configurações do terminal, neste caso é usado para restaurar às configurações inciais
			// TCSANOW faz com que as configurações sejam alteradas imediatamente
			tcsetattr(STDIN_FILENO, TCSANOW, &original);
		}
		escrever("\033[?25h", 6);   // mostra o cursor
	}

	// Função chamada pelo próprimo sistema quando Ctrl+C é pressionado o quando recebe SIGTERM (sinal de encerramento)
	// Restaura o terminal e deixa o sinal seguir o caminho normal (encerrar o programa)
	// Sem isso o terminal ficaria sem eco depois que o jogo fechasse
	void aoReceberSinal(int sinal)
	{
		restaurar();
		std::signal(sinal, SIG_DFL); // SIG_DFL faz o sinal voltar ao comportamento padrão, para que não seja mais tratado por esta função
		std::raise(sinal); // reenvia o sinal recebido, mas agora ele é tratado comportamento padrão do S.O.
	}

	// Tenta ler um byte, esperando no máximo "espera" microssegundos.
	// Devolve 1 se leu, 0 se não chegou nada e -1 se a entrada acabou
	int lerByte(char& c, int espera)
	{
		fd_set conjunto;
		FD_ZERO(&conjunto); // limpa conjuto antes de ser utilizado	
		FD_SET(STDIN_FILENO, &conjunto);

		struct timeval tempo;
		tempo.tv_sec = 0;
		tempo.tv_usec = espera;

		if (select(STDIN_FILENO + 1, &conjunto, nullptr, nullptr, &tempo) <= 0)
		{
			return 0;   // nada chegou (ou a espera foi interrompida)
		}

		ssize_t n = ::read(STDIN_FILENO, &c, 1);
		if (n == 1)
		{
			return 1;
		}
		if (n < 0 && errno == EINTR) // leitura falhou pois foi interrompida por um sinal
		{
			return 0;
		}
		return -1;   // n == 0: a entrada foi fechada
	}
}

// Liga o modo "cru": o terminal entrega cada tecla na hora (sem esperar Enter) e não mostra o que é digitado.
// O Ctrl+C continua funcionando. Também esconde o cursor
void Input::init()
{
	if (ativo)
	{
		return;
	}
	ativo = true;

	// Só mexe no terminal se a entrada for mesmo um terminal (tcgetattr falha em um pipe ou arquivo)
	if (tcgetattr(STDIN_FILENO, &original) == 0)
	{
		struct termios cru = original;
		cru.c_lflag &= ~(ICANON | ECHO);   // ICANON: esperar Enter. ECHO: mostrar o que foi digitado
		cru.c_cc[VMIN] = 0;                // read devolve na hora, mesmo sem tecla
		cru.c_cc[VTIME] = 0;
		tcsetattr(STDIN_FILENO, TCSANOW, &cru);
		terminalAlterado = true;
	}

	escrever("\033[?25l", 6);   // esconde o cursor

	// Qualquer jeito de sair (Ctrl+C, kill, fim normal do programa) restaura o terminal
	std::signal(SIGINT, aoReceberSinal); // se receber um sinal de encerramento, executa aoReceberSinal
	std::signal(SIGTERM, aoReceberSinal); // que restaura o terminal para a config padrão
	std::atexit(restaurar);
}

void Input::restore()
{
	restaurar();
}

// Pega a próxima tecla, se já tiver chegado. Não espera.
// As setas chegam do terminal como uma sequência de bytes (ESC [ A, ESC [ B...) e viram w, s, d, a.
// As outras sequências (Delete, F1...) são lidas inteiras e ignoradas, para não virarem teclas soltas
Input::Result Input::read(char& key)
{
	char c;
	int r = lerByte(c, 0);
	if (r < 0)
	{
		return END_OF_INPUT;
	}
	if (r == 0)
	{
		return NO_KEY;
	}

	if (c != 27)   // 27 = ESC
	{
		key = c;
		return KEY;
	}

	// ESC: início de uma sequência (setinhas). Os outros bytes chegam junto, então a espera é curta (5 ms).
	// Um ESC sozinho é ignorado
	char b;
	if (lerByte(b, 5000) != 1 || (b != '[' && b != 'O'))
	{
		return NO_KEY;
	}

	if (b == 'O')
	{
		// ESC O x: um byte final
		if (lerByte(b, 5000) != 1)
		{
			return NO_KEY;
		}
	}
	else
	{
		// ESC [ parâmetros letra: lê até o byte final (de 0x40 a 0x7E), por exemplo o A de ESC [ A
		do
		{
			if (lerByte(b, 5000) != 1)
			{
				return NO_KEY;
			}
		} while (b < 0x40 || b > 0x7E);
	}

	switch (b)
	{
		case 'A': key = 'w'; return KEY;   // seta para cima
		case 'B': key = 's'; return KEY;   // seta para baixo
		case 'C': key = 'd'; return KEY;   // seta para a direita
		case 'D': key = 'a'; return KEY;   // seta para a esquerda
		default:  return NO_KEY;
	}
}

// Espera até chegar uma tecla (ou a entrada acabar)
Input::Result Input::wait(char& key)
{
	while (true)
	{
		Result resultado = read(key);
		if (resultado != NO_KEY)
		{
			return resultado;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(5)); // espera 5 ms antes de tentar ler de novo
	}
}
