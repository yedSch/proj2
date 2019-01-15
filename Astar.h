//
// Created by yedaya on 1/15/19.
//

#ifndef PROJ2_ASTAR_H
#define PROJ2_ASTAR_H


#include "Searcher.h"

template <class T>
class AStar : public Searcher<T> {
    list<State<T> *> *search(Searchable<T> *searchable);

private:

    double fromHere(State<T> *state, State<T> *goal);
};


template<class T>
double AStar<T>::fromHere(State<T> *state, State<T>* goal) {
    Utils utils;
    return utils.distance(state->getState(), goal->getState());
}

template<class T>
list<State<T>*>* AStar<T>::search(Searchable<T>* searchable) {
// list of nodes that need to be checked
list<State<T>*> openList;
//list of nodes that have been checked
list<State<T>*> closedList;
map<State<T>*, double> toState;

State<T>* init = searchable->getInitialState();
State<T>* goal = searchable->getGoalState();

openList.push_back(init);
toState[init]=0;

while(!openList.empty()){
// greedy choice
State<T>* best= openList.front();
for(auto &o : openList){
if(this->fromHere(o,goal)+toState.at(o)<this->fromHere(best,goal)+toState.at(best)){
best=o;
}
}
openList.remove(best);
++this->evaluatedNodes;

list<State<T>*>* adj = searchable->getAllPossibleStates(best);
for(auto &a : *adj) {
if (a == goal) {
a->setCameFrom(best);
return this->backTrace(a, searchable);
}

// if in the closed list ignore
bool isInClosed = false;
for (auto &c : closedList) {
if (c == a) {
isInClosed = true;
break;
}
}
if (isInClosed) {
continue;
}

// if already in open list, check if the total score
// when we use the current generated path to get there
// if it is, update its score and parent
bool isInOpen = false;
for (auto &o : openList) {
if (o == a) {
isInOpen = true;
break;
}
}
if (isInOpen) {
if (this->fromHere(best,goal) + toState[best] + a->getCost() < this->fromHere(a,goal) + toState[a]) {
a->setCameFrom(best);
toState[a]=toState[best]+a->getCost();
best = a;
}
// if not in the open list add it and compute its score
} else {
a->setCameFrom(best);
toState[a]=toState[best]+a->getCost();
openList.push_back(a);
}
}
closedList.push_back(best);
}
}

#endif //PROJ2_ASTAR_H
