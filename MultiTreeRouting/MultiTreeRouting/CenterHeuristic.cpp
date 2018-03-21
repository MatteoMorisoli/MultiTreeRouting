//
//  CenterHeuristic.cpp
//  MultiTreeRouting
//
//  Created by Matteo Morisoli on 21.03.18.
//  Copyright © 2018 MatteoMorisoli. All rights reserved.
//

#include "CenterHeuristic.hpp"

std::set<int> CenterHeuristic::selectStartingNodes(const int numVertices, const Graph& graph, const int numRoots)const{
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
    for(int i = 0; i < numRoots; ++i){
        auto it = max_element(std::begin(num2StepNeighbours), std::end(num2StepNeighbours));
        startNodes.insert(std::distance(num2StepNeighbours.begin(), it));
        *it = 0;
    }
    return startNodes;
}
