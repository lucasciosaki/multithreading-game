#include "Entities.hpp"
#include "Board.hpp"

using namespace std;

//Essa file .cpp precisou ser criada para resolver a dependecia circular de Board e Entity

//Função para checar colisões e mover o Player dentro do tabuleiro
bool Player::move(Direction dir, Board &board, mutex &mboard, mutex &mscore){
    
    int collision; 

    //Precisei criar esse escopo para conseguir destravar o mutex antes de acessar a RC do Score
    {
    //Reserva o Mutex do tabuleiro para acessar a região crítica
    lock_guard<mutex> lock(mboard);

        //Checa colisões
        collision = board.isFreeOrGoal(dir, pos);
        if(collision == -1) return false;
        
        //Move o Player
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
    }
    
    //Se colisão, incrementa Score(RC)
    if(collision == 1) incrementScore(mscore);
    
    return true;
}