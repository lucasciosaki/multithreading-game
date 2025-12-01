# WHO TAKES TWO? - Jogo Multithreading
Este projeto consiste em uma implementação de um jogo Top-Down, com 2 players. O processo é dividido 3 threads, uma main que coleta os inputs e renderiza o jogo e uma thread para a logica de cada player.

## Explicando o Projeto

### Threads
A ideia de utilizar 3 threads busca trazer independência entre as lógicas dos jogadores e da renderização\
Para ser sincero, não traz nenhum ganho de desempenho. A questão é que se algum Player demorar muito em seu algoritmo, o outro Player não precisa ficar esperando este algoritmo acabar.\
Já a questão da thread de renderização estar desacoplada, é bem util, pois os players podem seguir uma taxa de atualização mais lenta enquanto a renderização segue uma mais rápida, parecendo mais fluido para o usuário e protegendo de gargalos visuais.

### Semáforos (Mutexes)

Neste projeto utilizamos apenas mutexes para proteger as seções críticas, que são listadas a seguir:
- *Grid*: O Tabuleiro é modificado tanto pela thread do P1 quanto pela do P2, para realizar seus movimentos, e são lidas pelo render na main
- *gameState*: O estado do jogo, assim como o grid, é modificado por P1 e P2 e lido pelo render
- *scores*: As pontuações de P1 e P2 são alteradas pelos mesmos, e lidas pelo render.
- *Direção P1*: A direção que P1 deve seguir é guardada em uma variável, sempre protegida por seu mutex, pois a Main produz as direções enquanto a thread do P1 consome
- *Direção P2*: A direção que P2 deve seguir é guardada em uma variável, sempre protegida por seu mutex, pois a Main produz as direções enquanto a thread do P2 consome
  
## Como Jogar?
Para o Player 1 (Azul) os comandos são:\
W -> Cima\
A -> Esquerda\
S -> Baixo\
D -> Direita

Para o Player 2 (Vermelho) os comandos são:\
Up_Arrow -> Cima\
Left_Arrow -> Esquerda\
Down_Arrow -> Baixo\
Right_Arrow -> Direita

Ganha o player que pegar dois objetivos primeiro

## Instalação

Recomendo utilizar Linux ou MacOS para instalar o jogo, pois não disponibilizei cmake, e a biblioteca SFML deverá ser instalada manualmente.
> Pode usar WSL, mas terá queda de desempenho

Para a intalação do projeto, basta seguir os seguintes passos:

1. Baixar o repositório e entrar no diretório:
```
git clone https://github.com/lucasciosaki/multithreading-game
cd ./multithreading-game
```
2. Instalar o SFML\
No Linux (Debian-Based):
```
sudo apt-get update
sudo apt-get install libsfml-dev
```
No MacOs:
```
brew install sfml
```

3. Compilar o código:\
No Linux
```
g++ main.cpp Entities.cpp -o wt2 -lsfml-graphics -lsfml-window -lsfml-system -pthread -std=c++17
```
No MacOS com chip M1/M2/M3
```
g++ main.cpp Entities.cpp -o wt2 -std=c++17 -I/opt/homebrew/include -L/opt/homebrew/lib -lsfml-graphics -lsfml-window -lsfml-system -pthread 
```
No MacOS com chip Intel
```
g++ main.cpp Entities.cpp -o wt2 -std=c++17 -lsfml-graphics -lsfml-window -lsfml-system -pthread
```

4. Agora basta rodar o jogo:
```
./wt2
```

