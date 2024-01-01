#ifndef TTT_AI_PERFECT_H
#define TTT_AI_PERFECT_H

#include "ArrayList.hpp"
#include "GameState.hpp"
#include "Graph.hpp"
#include "Vertex.hpp"
#include <stdexcept>
namespace ai_perfect
{

#define MAX_SEARCH_STATES 101999

// Call if anything needs to be setup one start (probably not)
void init()
{

}
int getReward(Vertex<GameState>* vert,enum PLAYER_ID player_id);
/*int findMax(ArrayList<int> list){
    int max=0;
    for(int i=0;i<list.size();i++){
        if(list[i]>max)
            max=list[i];
    }
    return max;
}
int findMin(ArrayList<int> list){
    if (list.size()==0){
        return 0;
    }
    int min=list[0];
    for(int i=0;i<list.size();i++){
        if (list[i]<min){
            min=list[i];
        }
    }
    return min;
}
int findMaxIndex(ArrayList<int> list){
    int max=0;
    int index=0;
    for(int i=0;i<list.size();i++){
        if(list[i]>max){
            max=list[i];
            index=i;
        }
    }
    return index;
}
int findMinIndex(ArrayList<int> list){
    if (list.size()==0){
        return 0;
    }
    int min=list[0];
    for(int i=0;i<list.size();i++){
        if (list[i]<min){
            min=list[i];
        }
    }
    return min;
}*/

Vec get_move(const GameState& state, enum PLAYER_ID player_id, bool cap_states){
    
    unsigned int current_states = 1;
    Graph<GameState> stateSpace;
    Vertex<GameState>* start = new Vertex<GameState>(state);

    stateSpace.addVertex(start);

    Queue<Vertex<GameState>*> frontier;
    frontier.enqueue(start);
    while (!frontier.isEmpty()&&(current_states<MAX_SEARCH_STATES||!cap_states)){
        Vertex<GameState>* curr = frontier.dequeue();
        if (!curr->data.done){
            for(int i = 0; i < state.size; i++){
                for (int j = 0; j < state.size; j++){
                    if(curr->data.grid[i][j] == -1){
                        GameState next = curr->data;
                        next.play(i, j);
                        Vertex<GameState>* successor = new Vertex<GameState>(next);
                        stateSpace.addVertex(successor);
                        stateSpace.addDirectedEdge(curr, successor);
                        current_states++;
                        if (!successor->data.done){
                            frontier.enqueue(successor);
                        }
                    }
                }
            }
        }
    }
    if(current_states>=MAX_SEARCH_STATES&&cap_states){
        return ai_random::get_move(state,player_id);
    }
    auto& list = start->neighbors;
    Vec bestmove = list[0]->location->data.lastMove;
    int bestreward = getReward(list[0]->location,player_id);
    for(int i=1;i<list.size();i++){
        int curr_reward=getReward(list[i]->location, player_id);
        if(curr_reward>bestreward){
            bestreward=curr_reward;
            bestmove=list[i]->location->data.lastMove;
        }
    }
    //finds which move gives the highest reward out of all those in question
    return bestmove;
}

int getReward(Vertex<GameState>* start, enum PLAYER_ID player_id){
//    std::srand(time(NULL));
    int score = (rand()%50) - 25;
    if (start->data.done){
        int currPlayer = start->data.currentTurn;
        if (start->data.hasWon(player_id)){
            score = 100;
        }
        else if (start->data.hasWon(!player_id)){
            score = -100;
        }
    }
    else{
        if(start->neighbors.size()==0){
            return score;
        }
        score = getReward(start->neighbors[0]->location, player_id);
        for (int i = 1; i < start->neighbors.size(); i++){
            int curr = getReward(start->neighbors[i]->location, player_id);
            if (start->data.currentTurn == player_id){
                if (curr > score){
                    score = curr;
                }
            }
            else{
                if (curr < score){
                    score = curr;
                }
            }
        }
    }
    return score;
}

// Call if needs to undo anything from init (probably not)
void deinit()
{

}

}
#endif
