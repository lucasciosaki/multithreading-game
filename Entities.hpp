#pragma once

using namespace std;
#include <vector>
#include <mutex>
#include <thread>
#include <random>

#include "Types.hpp"

class Board;

class Entity{
    
    public:
        Position pos;
        EntityType type;
        Entity(int x, int y, EntityType type = EntityType::EMPTY){
            pos.x = x;
            pos.y = y;
            this->type = type; 
        }

};

class Goal: public Entity{
    public: 
        Goal(int x, int y) : Entity(x, y, type){
            type = EntityType::GOAL;
        }
};

class Player: public Entity{
    private:
        int score = 0;
    public:
        

        Player(int x, int y, EntityType type) : Entity(x, y, type){
        }

        bool move(Direction dir, Board &board, mutex &mboard, mutex &mscore);

        int getPlayerScore(mutex &mscore){
            lock_guard lock(mscore);
            return score;
        }

        void incrementScore(mutex &mscore){
            lock_guard lock(mscore);
            score++;
        }

        void restartScore(mutex &mscore){
            lock_guard lock(mscore);
            score = 0;
        }

};

