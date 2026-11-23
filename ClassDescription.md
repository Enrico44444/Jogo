# Headers do projeto

- `Position.h`: guarda uma posição (x, y) e calcula a posição à frente.
- `MoveResult.h`: define os resultados possíveis de um movimento (andou, bloqueado, porta).
- `Character.h`: base comum de quem tem vida e posição, e pode se mover e receber dano.
- `Player.h`: o jogador, que anda, atira, se cura e pega itens.
- `Zombie.h`: o inimigo, que persegue e ataca o jogador e volta à posição inicial.
- `Weapon.h`: a arma (pistola, escopeta ou rifle), com dano, alcance e munição no carregador, que atira e recarrega.
- `Item.h`: base comum dos itens, com nome, descrição, quantidade e um efeito ao usar.
- `HealItem.h`: item que cura o jogador.
- `AmmoItem.h`: munição de um tipo; a quantidade do item é o número de balas, usadas ao recarregar a arma.
- `WeaponItem.h`: arma no chão ou no inventário, que o jogador equipa ao usar.
- `KeyItem.h`: chave que abre portas trancadas.
- `Inventory.h`: guarda os itens do jogador e permite adicionar, remover e usar.
- `Tile.h`: base comum das células do mapa.
- `Floor.h`: piso onde se pode andar.
- `Wall.h`: parede que bloqueia o caminho.
- `Door.h`: porta que leva a outra sala e pode estar trancada.
- `MapSection.h`: a sala atual, lida de um arquivo `.txt`, com seus tiles, zumbis, itens e portas.
- `Game.h`: controla o jogo: lê a tecla, atualiza o estado, desenha a tela e troca de sala.
