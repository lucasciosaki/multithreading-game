#include "Entities.hpp"
#include "Board.hpp"

using namespace std;

bool Player::move(Direction dir, Board &board, mutex &mboard, mutex &mscore){
    lock_guard<mutex> lock(mboard);
            int collision = board.isFreeOrGoal(dir, pos);
            if(collision == -1) return false;
            
            switch (dir)
            {
                case Direction::DOWN:             
                    board.grid[pos.x][pos.y + 1] = type;
                    board.grid[pos.x][pos.y] = EntityType::EMPTY;
                    pos.y += 1;
                    break;
                
                case Direction::UP:
                    board.grid[pos.x][pos.y - 1] = type;
                    board.grid[pos.x][pos.y] = EntityType::EMPTY;
                    pos.y -= 1;
                    break;

                case Direction::LEFT:
                    board.grid[pos.x - 1][pos.y] = type;
                    board.grid[pos.x][pos.y] = EntityType::EMPTY;
                    pos.x -= 1;
                    break;

                case Direction::RIGHT:
                    board.grid[pos.x + 1][pos.y] = type;
                    board.grid[pos.x][pos.y] = EntityType::EMPTY;
                    pos.x += 1;
                    break;
                    
                default:
                    break;
            }

            if(collision == 1) incrementScore(mscore);
            return true;
}