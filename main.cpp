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

Direction directionP1 = Direction::IDLE;
mutex mdirectionP1;


Direction getDirectionP1(){
    lock_guard lock(mdirectionP1);
    return directionP1;
}

void setDirectionP1(Direction direction){
    lock_guard lock(mdirectionP1);
    directionP1 = direction;
    return;
}

Direction directionP2 = Direction::IDLE;
mutex mdirectionP2;


Direction getDirectionP2(){
    lock_guard lock(mdirectionP2);
    return directionP2;
}

void setDirectionP2(Direction direction){
    lock_guard lock(mdirectionP2);
    directionP2 = direction;
    return;
}

void threadPlayer1(Game &game){

    Direction last_dir = Direction::IDLE; 
    bool isRepeating = false;

    while(game.curGameState() != GameState::EXIT){
        if(game.curGameState() == GameState::RUNNING){
            Direction dir = getDirectionP1();
            if(dir != Direction::IDLE){
                if(last_dir != dir){
                    isRepeating = false;
                    last_dir = dir;
                }

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
    }
}

void threadPlayer2(Game &game){
    
    Direction last_dir = Direction::IDLE; 
    bool isRepeating = false;

    while(game.curGameState() != GameState::EXIT){

        if(game.curGameState() == GameState::RUNNING){
            Direction dir = getDirectionP2();

            if(dir != Direction::IDLE){
                if(last_dir != dir){
                    isRepeating = false;
                    last_dir = dir;
                }

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
    }
}

int main(){

    int width = 15;
    int height = 10;
    int tileSize = 50;
    sf::RenderWindow window(sf::VideoMode(width*tileSize, height*tileSize), "WHO TAKES 2?");

    Game game = Game(width, height);

    Renderer renderer(window);

    thread threadp1(threadPlayer1, ref(game));
    thread threadp2(threadPlayer2, ref(game));

    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
        }

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


        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && game.curGameState() != GameState::RUNNING){
            game.restart();
        }

        window.clear();
        
        
        game.render(renderer);
        
        window.display();
    }
    game.setGameState(GameState::EXIT);
    threadp1.join();
    threadp2.join();

    return 0;
}

