# Jogo

Jogo de terminal em C++ no estilo de Resident Evil 1: você anda por salas desenhadas em texto, pega armas, munição, ervas e chaves, abre portas e enfrenta zumbis.

> **Funciona apenas em Linux.** A leitura do teclado (`src/Input.cpp`) usa `termios`, `select` e `unistd.h`, que são específicos de sistemas POSIX, e a tela é desenhada com códigos de escape ANSI. O jogo **não compila no Windows** nativo.

## Requisitos

- Linux
- `g++` com suporte a C++11 ou superior
- `make`
- As bibliotecas AddressSanitizer e LeakSanitizer (o Makefile compila com `-fsanitize=address -fsanitize=leak`)

## Como compilar e rodar

Abra um terminal **dentro da pasta do projeto** e execute:

```bash
make
make run
```

Para apagar o executável: `make clean`.

## Controles

| Tecla | Ação |
|---|---|
| Setas ou `w` `a` `s` `d` | Andar (e virar para a direção) |
| `f` | Atirar na direção em que o jogador está virado |
| `r` | Recarregar a arma com a munição do inventário |
| `g` | Guardar a arma equipada no inventário |
| `1` a `8` | Usar o item de mesmo número do inventário (equipar arma, usar erva) |
| `s` / `n` | Responder à pergunta "Entrar na porta?" |
| `q` | Sair |

## Como jogar

- **`P`** é você, **`Z`** são os zumbis, **`D`** são portas e **`#`** são paredes.
- Itens no chão: **`H`** erva (cura), **`A`** munição, **`K`** chave, **`W`** arma. Ande por cima para pegar. O inventário tem 8 vagas.
- Para atirar, pegue uma arma, use-a pelo número no inventário para equipar e aperte `f`. O tiro vai até o alcance da arma e é bloqueado por paredes e portas.
- Armas: **pistola** (equilibrada), **escopeta** (dano alto, curto alcance) e **rifle** (longo alcance, poucas balas). Cada uma usa a sua munição.
- Portas trancadas pedem a chave correspondente, que fica em alguma sala. Ande até a porta para usá-la.
- Se a vida chegar a zero, o jogo termina.
- A **primeira sala não tem zumbis**: use-a para aprender os controles e pegar a pistola.

## Atenção: o jogo grava nos arquivos das salas

Ao pegar um item, destrancar uma porta ou matar um zumbi, o jogo **altera o arquivo `.txt` da sala** para que a mudança continue valendo quando você voltar. Por isso, uma partida modifica os arquivos em `MapSections/`. Para recomeçar do zero, restaure a pasta `MapSections` a partir do zip original (ou do repositório).

## Estrutura

```
include/       cabeçalhos (.h)
src/           código-fonte (.cpp)
MapSections/   salas do jogo (map1.txt a map5.txt)
ClassDescription.md   descrição de cada classe
Makefile
```

## Criando ou editando salas

Cada sala é um `.txt`: a primeira linha é `largura altura`, depois vem a grade do mapa e, por fim, linhas que descrevem os objetos:

```
PLAYER x y
ZOMBIE x y
DOOR x y arquivo_destino x_chegada y_chegada [id_da_chave]
HEAL x y quantidade
AMMO x y PISTOL|SHOTGUN|RIFLE balas
WEAPON x y PISTOL|SHOTGUN|RIFLE [balas]
KEY x y id
```

Cada `D`, `H`, `A`, `K` e `W` da grade precisa ter a sua linha. Se houver erro no arquivo, o jogo mostra a linha com o problema. Salas pequenas não devem ter zumbis.