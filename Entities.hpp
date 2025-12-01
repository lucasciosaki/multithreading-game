#pragma once

using namespace std;
#include <vector>
#include <mutex>
#include <thread>
#include <random>

#include "Types.hpp"



class Board;

//SuperClasse Entity
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

//Classe para representar os Goals(Objetivos)
class Goal: public Entity{
    public: 
        Goal(int x, int y) : Entity(x, y, type){
            type = EntityType::GOAL;
        }
};

//Classe que representa os Players
class Player: public Entity{
    private:
        int score = 0;
    public:
        

        Player(int x, int y, EntityType type) : Entity(x, y, type){
        }

        bool move(Direction dir, Board &board, mutex &mboard, mutex &mscore);


        // Retorna o Score do Player
        int getPlayerScore(mutex &mscore){
            //Acessa a região crítica do Score
            lock_guard lock(mscore);
            return score;
        }

        //Incrementa o Score do Player
        void incrementScore(mutex &mscore){
            //Acessa a região crítica do Score
            lock_guard lock(mscore);
            score++;
        }

        //Zera o Score do Player
        void restartScore(mutex &mscore){
            //Acessa a região crítica do Score
            lock_guard lock(mscore);
            score = 0;
        }

};

