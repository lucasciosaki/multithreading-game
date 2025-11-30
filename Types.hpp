#pragma once

using namespace std;




struct Position{
    int x;
    int y;

    bool operator==(const Position& other) const {
        return x == other.x && y == other.y;
    }

    double distance(const Position& other) const{
        double deltax = x - other.x;
        double deltay = y - other.y;

        return sqrt(deltax*deltax + deltay*deltay);
    }
};



enum class GameState{
    START, RUNNING, P1WINS, P2WINS, EXIT
};

enum class Direction{
    IDLE, UP, DOWN, RIGHT, LEFT
};

enum class EntityType{
    EMPTY, PLAYER1, PLAYER2, GOAL 
};