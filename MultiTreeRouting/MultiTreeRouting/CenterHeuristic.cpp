//
//  CenterHeuristic.cpp
//  MultiTreeRouting
//
//  Created by Matteo Morisoli on 21.03.18.
//  Copyright © 2018 MatteoMorisoli. All rights reserved.
//

#include "CenterHeuristic.hpp"

CenterHeuristic::CenterHeuristic(int numV): Heuristic(numV){}

int CenterHeuristic::selectStartingNode(const Graph& graph){
    std::vector<int> numNeighbours;
    std::vector<int> num2StepNeighbours;
    std::set<int> startNodes;
    Graph::adjacency_iterator neighStart, neighEnd;
    for(int i = 0; i < numVertices; ++i){
        tie(neighStart, neighEnd) = boost::adjacent_vertices(i, graph);
        numNeighbours.emplace_back(std::distance(neighStart, neighEnd));
    }
    for(int i = 0; i < numVertices; ++i){
        int sum = numNeighbours[i];
        tie(neighStart, neighEnd) = boost::adjacent_vertices(i, graph);
        for(; neighStart != neighEnd; ++neighStart){
            sum += numNeighbours[*neighStart];
        }
        num2StepNeighbours.emplace_back(sum);
    }
    auto it = max_element(std::begin(num2StepNeighbours), std::end(num2StepNeighbours));
    int root = std::distance(num2StepNeighbours.begin(), it);
    while(std::find(this->usedRoots.begin(), this->usedRoots.end(), root) != this->usedRoots.end()){
        *it = 0;
        it = max_element(std::begin(num2StepNeighbours), std::end(num2StepNeighbours));
        root = std::distance(num2StepNeighbours.begin(), it);
    }
    this->usedRoots.push_back(root);
    return root;
}
