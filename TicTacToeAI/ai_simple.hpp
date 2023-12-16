#ifndef TTT_AI_SIMPLE_H
#define TTT_AI_SIMPLE_H

#include "GameState.hpp"

namespace ai_simple
{
// Call if anything needs to be setup on game start (probably not)
void init()
{

}

Vec get_move(const GameState& state, enum PLAYER_ID player_id){
    for (int i = 0; i < state.size; i++){
        for (int j = 0; j < state.size; j++){
            if (state.grid[i][j] == -1){
                return Vec(i, j);
            }
        }
    }
    return Vec(0,0);
}
// Call if needs to undo anything from init (probably not)
void deinit()
{

}

}
#endif 