#pragma once

using namespace std;
#include <vector>
#include <mutex>
#include <thread>
#include <random>

#include "Types.hpp"
#include "Entities.hpp"

class Board{
    public:
        vector<vector<EntityType>> grid;
        int width, height;

        Board(int width, int height){
            this->width = width;
            this->height = height;
            grid.resize(width, (vector<EntityType>(height, EntityType::EMPTY)));
        };

        void clear(mutex &mboard){
            lock_guard lock(mboard);
            grid.assign(width, vector<EntityType>(height, EntityType::EMPTY));
        }

        //Retorna -1 se for colisão, 0 se for livre e 1 se for goal
        int isFreeOrGoal(Direction dir, Position pos){
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

        bool insertEntity(Entity &entity, mutex &mboard){
            lock_guard<mutex> lock(mboard);

            if(entity.pos.x < 0 || entity.pos.x >= width || entity.pos.y < 0 || entity.pos.y >= height){
                return false;
            }
            grid[entity.pos.x][entity.pos.y] = entity.type;
            return true;
        }

        bool isEmpty(Position pos, mutex &m){
            lock_guard lock(m);
            return grid[pos.x][pos.y] == EntityType::EMPTY;
        }

};  
