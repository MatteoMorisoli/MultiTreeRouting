//
//  SimpleHeuristic.cpp
//  MultiTreeRouting
//
//  Created by Matteo Morisoli on 13.03.18.
//  Copyright © 2018 MatteoMorisoli. All rights reserved.
//

#include "SimpleHeuristic.hpp"

SimpleHeuristic::SimpleHeuristic(int numV): Heuristic(numV){}

int SimpleHeuristic::selectStartingNode(const Graph& graph){
    boost::random::uniform_int_distribution<> dist(0, this->numVertices -1);
    boost::random::mt19937 gen(std::time(0));
    int root = dist(gen);
    while(std::find(this->usedRoots.begin(), this->usedRoots.end(), root) != this->usedRoots.end()){
        root = dist(gen);
    }
    this->usedRoots.push_back(root);
    return root;
}
