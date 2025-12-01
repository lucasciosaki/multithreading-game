#include <iostream>
#include <vector>
#include <mutex>
#include <thread>
#include <random>
#include <SFML/Graphics.hpp>
#include "Types.hpp"
#include "Entities.hpp"
#include "Board.hpp"
#include "Game.hpp"
#include "Renderer.hpp"

using namespace std;

//Buffer de direção do Player 1 e seu mutex
Direction directionP1 = Direction::IDLE;
mutex mdirectionP1;

//Retorna o valor no buffer da direção do P1
Direction getDirectionP1(){
    lock_guard lock(mdirectionP1);
    return directionP1;
}

//Altera valor no buffer da direção do P1
void setDirectionP1(Direction direction){
    lock_guard lock(mdirectionP1);
    directionP1 = direction;
    return;
}

//Direção do Player 2 e seu mutex
Direction directionP2 = Direction::IDLE;
mutex mdirectionP2;

//Retorna o valor no buffer da direção do P2
Direction getDirectionP2(){
    lock_guard lock(mdirectionP2);
    return directionP2;
}

//Altera valor no buffer da direção do P2
void setDirectionP2(Direction direction){
    lock_guard lock(mdirectionP2);
    directionP2 = direction;
    return;
}

//Thread do P1
void threadPlayer1(Game &game){

    Direction last_dir = Direction::IDLE; 
    bool isRepeating = false;


    while(true){
        //Recebe estado atual do jogo
        GameState curState = game.curGameState();

        //Condição de saida do loop
        if (curState == GameState::EXIT) {
            break;
        }

        if(curState == GameState::RUNNING){
            //Pega o valor do buffer
            Direction dir = getDirectionP1();

            //Logica para deixar a movimentação mais precisa
            if(dir != Direction::IDLE){
                if(last_dir != dir){
                    isRepeating = false;
                    last_dir = dir;
                }

                //Chama a lógica do P1 de fato
                game.tickP1(dir);
                
                if(!isRepeating){
                    this_thread::sleep_for(chrono::milliseconds(250));
                    isRepeating = true;
                }
                else{
                    this_thread::sleep_for(chrono::milliseconds(100));
                }
            }
            else {
            
                isRepeating = false;
                last_dir = Direction::IDLE;
                
                
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        }
        else{
            //Se não estiver RUNNING, sleep para gastar menos CPU
            this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
}

//Thread do P2
void threadPlayer2(Game &game){
    
    Direction last_dir = Direction::IDLE; 
    bool isRepeating = false;

    while(true){

        //Recebe estado atual do jogo
        GameState curState = game.curGameState();

        //Condição de saida do loop
        if(curState == GameState::EXIT){
            break;
        }

        if(curState == GameState::RUNNING){
            
            //Pega o valor do buffer
            Direction dir = getDirectionP2();


            //Logica para deixar a movimentação mais precisa
            if(dir != Direction::IDLE){
                if(last_dir != dir){
                    isRepeating = false;
                    last_dir = dir;
                }

                //Chama a lógica do P2 de fato
                game.tickP2(dir);
                
                if(!isRepeating){
                    isRepeating = true;
                    this_thread::sleep_for(chrono::milliseconds(250));
                }
                else{
                    this_thread::sleep_for(chrono::milliseconds(100));
                }

            }
            else {
                
                isRepeating = false;
                last_dir = Direction::IDLE;
                
            
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        }
        else{
            this_thread::sleep_for(chrono::milliseconds(100));
        }
    }
}

int main(){

    int width = 15;
    int height = 10;
    int tileSize = 50;

    //Cria janela do SFML
    sf::RenderWindow window(sf::VideoMode(width*tileSize, height*tileSize), "WHO TAKES 2?");

    //Cria o jogo
    Game game = Game(width, height);

    //Cria renderizador
    Renderer renderer(window);


    //Lança as threads de P1 e P2
    thread threadp1(threadPlayer1, ref(game));
    thread threadp2(threadPlayer2, ref(game));

    while (window.isOpen()) {
        sf::Event event;


        //Fecha janela
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
        }


        //Identifica teclas do P1 e insere no buffer
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            setDirectionP1(Direction::UP);
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            setDirectionP1(Direction::DOWN);
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            setDirectionP1(Direction::RIGHT);
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            setDirectionP1(Direction::LEFT);
        else
            setDirectionP1(Direction::IDLE);


        //Identifica teclas do P2 e insere no buffer
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            setDirectionP2(Direction::UP);
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            setDirectionP2(Direction::DOWN);
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            setDirectionP2(Direction::RIGHT);
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            setDirectionP2(Direction::LEFT);
        else
            setDirectionP2(Direction::IDLE);


        //Identifica Start e Restart
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && game.curGameState() != GameState::RUNNING){
            game.restart();
        }

        //Limpa janela
        window.clear();
        
        //Renderiza
        game.render(renderer);
        window.display();
    }
    //Quando janela fechada, set gameState como EXIT e da join nas threads 
    game.setGameState(GameState::EXIT);
    threadp1.join();
    threadp2.join();

    return 0;
}

