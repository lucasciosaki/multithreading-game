#pragma once

using namespace std;
#include <iostream>
#include <vector>
#include <mutex>
#include <thread>
#include <random>
#include <SFML/Graphics.hpp>

#include "Types.hpp"
#include "Entities.hpp"
#include "Board.hpp"
#include "Renderer.hpp"

class Game{

    private:
        GameState state;

        void createGoals(){
            //Biblioteca Random, para gerar posicoes aleatorias para os goals
            random_device rd;
            mt19937 gen(rd());
            uniform_int_distribution<> distWidth(board.width/3  , (board.width/3 * 2) - 1);
            uniform_int_distribution<> distHeight(board.height/3 , (board.height/3 * 2) - 1);
            
            //Logica de criacao de Goals, e insercao em locais aleatorios diferentes
            
            while(goals.size() < 3){
                Position pos;
                pos.x = distWidth(gen);
                pos.y = distHeight(gen);


                for(size_t j = 0; j < goals.size(); j++){
                    if(pos.distance(goals[j].pos) <= 2){
                        pos.x = distWidth(gen);
                        pos.y = distHeight(gen);
                        j = 0;
                    }
                }
                
                if(!board.isEmpty(pos, mboard)) continue;

                Goal goal = Goal(pos.x, pos.y);
                goals.push_back(goal);
                board.insertEntity(goal, mboard);
            }
        }

    public:
        
        mutable mutex mstate;
        mutable mutex mboard;
        mutable mutex mscorep1;
        mutable mutex mscorep2;

        Board board;
        Player p1;
        Player p2;
        vector<Goal> goals;

        GameState curGameState(){
            lock_guard lock(mstate);
            return state;
        }

        void setGameState(GameState new_state){
            lock_guard lock(mstate);
            state = new_state;
            return;
        }

        Game(int width, int height) : 
        board(width, height),
        p1(0, 0, EntityType::PLAYER1),
        p2(width - 1, height - 1, EntityType::PLAYER2)
        {
            board.insertEntity(p1, mboard);
            board.insertEntity(p2, mboard);

            createGoals();
            setGameState(GameState::START);
        }

        

        void restart(){
            board.clear(mboard);
            
            p1.pos.x = 0;
            p1.pos.y = 0;
            p1.restartScore(mscorep1);

            p2.pos.x = board.width - 1;
            p2.pos.y = board.height - 1;
            p2.restartScore(mscorep2);

            board.insertEntity(p1, mboard);
            board.insertEntity(p2, mboard);

            goals.clear();
            createGoals();
            setGameState(GameState::RUNNING);
        }

        void tickP1(Direction direction){
            
            if(direction != Direction::IDLE)
                p1.move(direction, board, mboard, mscorep1);
            if(p1.getPlayerScore(mscorep1) == 2){

                setGameState(GameState::P1WINS);        
            }
            
        }

        void tickP2(Direction direction){
            if(direction != Direction::IDLE)
                p2.move(direction, board, mboard, mscorep2);
            if(p2.getPlayerScore(mscorep2) == 2){
                setGameState(GameState::P2WINS);
            }
        }
        
        void render(Renderer& renderer) {
            
            std::lock_guard<std::mutex> lock(mboard);
            
            if(curGameState() == GameState::START){
                renderer.drawGrid(board.grid);
                renderer.drawStart();
            }
            else if(curGameState() == GameState::RUNNING){
                renderer.drawGrid(board.grid);
                renderer.drawP1Score(p1.getPlayerScore(mscorep1));
                renderer.drawP2Score(p2.getPlayerScore(mscorep2));
            }
            else if(curGameState() == GameState::P1WINS){
                renderer.drawGrid(board.grid);
                renderer.drawWinner(GameState::P1WINS);
            }
            else if(curGameState() == GameState::P2WINS){
                renderer.drawGrid(board.grid);
                renderer.drawWinner(GameState::P2WINS);
                renderer.drawStart();
            }
        } 

};