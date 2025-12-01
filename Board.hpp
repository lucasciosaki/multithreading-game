#pragma once

using namespace std;
#include <vector>
#include <mutex>
#include <thread>
#include <random>

#include "Types.hpp"
#include "Entities.hpp"


//Classe para representar o tabuleiro
class Board{
    public:
        //Matriz de EntityTipes, representa o estado atual do tabuleiro
        vector<vector<EntityType>> grid;

        //Largura e Altura do Tabuleiro
        int width, height;

        //Construtor, inicializa o tabuleiro com EntityType::EMPTY
        Board(int width, int height){
            this->width = width;
            this->height = height;
            grid.resize(width, (vector<EntityType>(height, EntityType::EMPTY)));
        };


        //Limpa a matriz do tabuleiro
        void clear(mutex &mboard){
            lock_guard lock(mboard);
            grid.assign(width, vector<EntityType>(height, EntityType::EMPTY));
        }

        //Retorna -1 se for houver colisão, 0 se for livre e 1 se for Goal
        int isFreeOrGoal(Direction dir, Position pos){
            
            //Para cada tipo de direção, verifica a colisão
            switch (dir)
            {
            case Direction::DOWN:
                if(pos.y + 1 >= height) return -1;
                if(grid[pos.x][pos.y + 1] == EntityType::EMPTY) return 0;
                if(grid[pos.x][pos.y + 1] == EntityType::GOAL) return 1;
                return -1;
    
            case Direction::UP:
                if(pos.y - 1 < 0) return -1;
                if(grid[pos.x][pos.y - 1] == EntityType::EMPTY) return 0;
                if(grid[pos.x][pos.y - 1] == EntityType::GOAL) return 1;
                return -1;
                break;
            
            case Direction::LEFT:
                if(pos.x - 1 < 0) return -1;
                if(grid[pos.x - 1][pos.y] == EntityType::EMPTY) return 0;
                if(grid[pos.x - 1][pos.y] == EntityType::GOAL) return 1;
                return -1;
                break;
            

            case Direction::RIGHT:
                if(pos.x + 1 >= width) return -1;
                if(grid[pos.x + 1][pos.y] == EntityType::EMPTY) return 0;
                if(grid[pos.x + 1][pos.y] == EntityType::GOAL) return 1;
                return -1;
                break;
                
            default:
                return -1;
                break;
            }
            return -1;
        }


        //Insere uma Entity no tabuleiro
        bool insertEntity(Entity &entity, mutex &mboard){
            lock_guard<mutex> lock(mboard);

            if(entity.pos.x < 0 || entity.pos.x >= width || entity.pos.y < 0 || entity.pos.y >= height){
                return false;
            }
            grid[entity.pos.x][entity.pos.y] = entity.type;
            return true;
        }

        //Retorna se uma determinada posição do tabuleiro está livre
        bool isEmpty(Position pos, mutex &m){
            lock_guard lock(m);
            return grid[pos.x][pos.y] == EntityType::EMPTY;
        }

};  
