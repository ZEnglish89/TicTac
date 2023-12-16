#ifndef TTT_AI_RANDOM_H
#define TTT_AI_RANDOM_H

#include "GameState.hpp"
#include "ArrayList.hpp"
namespace ai_random
{



// Call if anything needs to be setup on game start (probably not)
void init()
{

}

Vec get_move(const GameState& state, enum PLAYER_ID player_id)
{
    ArrayList<ArrayList<int>> valid;
    for(int i=0;i<state.size;i++){
        for(int j=0;j<state.size;j++){
            if(state.grid[i][j]==-1){
                ArrayList<int> space;
                space.append(i);
                space.append(j);
                valid.append(space);
            }
        }
    }
    std::srand((unsigned)time(NULL));
    int random=rand()%(valid.size());
    return Vec(valid[random][0],valid[random][1]);
}

// Call if needs to undo anything from init (probably not)
void deinit()
{

}

}
#endif 