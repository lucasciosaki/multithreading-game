#pragma once

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

using namespace std;

class Renderer{
    private:
        sf::RenderWindow& window;
        sf::RectangleShape shapeGoal;
        sf::RectangleShape shapeP1;
        sf::RectangleShape shapeP2;
        sf::RectangleShape shapeFloor;
        sf::Font font;
        

    public:
        Renderer(sf::RenderWindow &win) : window(win){
            font.loadFromFile("./src/font.ttf");

            float size = 50.0f; // Tamanho do tile
        
            shapeGoal.setSize({size, size});
            shapeGoal.setFillColor(sf::Color::White);

            shapeP1.setSize({size, size});
            shapeP1.setFillColor(sf::Color::Blue);

            shapeP2.setSize({size, size});
            shapeP2.setFillColor(sf::Color::Red);

            shapeFloor.setSize({size, size});
            shapeFloor.setFillColor(sf::Color(30, 30, 30)); // Cinza escuro
        };

        void drawP1Score(const int &score){
            sf::Text p1score;
            p1score.setFont(font);
            string str = "Player 1:" + to_string(score);
            p1score.setString(str);

            p1score.setCharacterSize(20);
            p1score.setFillColor(sf::Color::Blue);

            p1score.setPosition(48, window.getSize().y - 48);
            window.draw(p1score);
        }
        
        void drawP2Score(const int &score){
            sf::Text p2score;
            p2score.setFont(font);

            string str = "Player 2:" + to_string(score);
            p2score.setString(str);

            p2score.setCharacterSize(20);
            p2score.setFillColor(sf::Color::Red);

            p2score.setPosition(window.getSize().x - 150, 24);
            window.draw(p2score);
        }

        void drawWinner(const GameState &state){
            

            sf::Text text;
            text.setFont(font);
            text.setCharacterSize(40);

            string winner;
            sf::RectangleShape block;
            
            if(state == GameState::P1WINS){
                winner = "PLAYER 1";
                text.setFillColor(sf::Color::Blue);
            }
            else if(state == GameState::P2WINS){
                winner = "PLAYER 2";
                text.setFillColor(sf::Color::Red);
            }

            winner.append(" WINS");

            text.setString(winner);

            text.setPosition(window.getSize().x / 2 - 150, window.getSize().y /2 - 48);

            block.setSize({450, 100});
            block.setPosition(window.getSize().x / 2 - 225, window.getSize().y/2 - 75);
            block.setFillColor(sf::Color::White);
            
            window.draw(block);
            window.draw(text);
        }

        void drawStart(){
            sf::Text text;
            text.setFont(font);
            text.setCharacterSize(40);
            text.setFillColor(sf::Color::Black);

            string str = "Press ENTER to start";
            text.setString(str);
            text.setPosition(window.getSize().x / 2 - 225, window.getSize().y/2 + 100);
            window.draw(text);
        }



        void drawGrid(const std::vector<std::vector<EntityType>>& grid) {
        int width = grid.size();
        int height = grid[0].size();

        for (int x = 0; x < width; ++x) {
            for (int y = 0; y < height; ++y) {
                EntityType cell = grid[x][y];
                float py = y * 50.0f;
                float px = x * 50.0f;

                // 1. Desenha chão sempre
                shapeFloor.setPosition(px, py);
                window.draw(shapeFloor);

                // 2. Desenha objeto por cima
                if (cell == EntityType::GOAL) { // Goal
                    shapeGoal.setPosition(px, py);
                    window.draw(shapeGoal);
                } 
                else if (cell == EntityType::PLAYER1) { // P1
                    shapeP1.setPosition(px, py);
                    window.draw(shapeP1);
                }
                else if (cell == EntityType::PLAYER2) { // P2
                    shapeP2.setPosition(px, py);
                    window.draw(shapeP2);
                }
            }
        }
    }
    

};

