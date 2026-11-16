#ifndef INPUT_H
#define INPUT_H

// Leitura do teclado
class Input
{
public:
	enum Result { NO_KEY, KEY, END_OF_INPUT };

	static void init();   // Liga o modo "cru" do teclado e esconde o cursor
	static void restore();   // Desfaz o init: restaura o terminal e mostra o cursor (pode ser chamada mais de uma vez)
	static Result read(char& key);   // Pega a próxima tecla, se já tiver chegado. Não espera
	static Result wait(char& key);   // Espera até chegar uma tecla

};

#endif
