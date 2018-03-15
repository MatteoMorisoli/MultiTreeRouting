//
//  SimpleHeuristic.cpp
//  MultiTreeRouting
//
//  Created by Matteo Morisoli on 13.03.18.
//  Copyright © 2018 MatteoMorisoli. All rights reserved.
//

#include "SimpleHeuristic.hpp"

std::set<int> SimpleHeuristic::selectStartingNodes(int numVertices, int numRoots){
    std::set<int> nodes;
    boost::random::uniform_int_distribution<> dist(0, numVertices -1);
    boost::random::mt19937 gen(std::time(0));
    while(nodes.size() < numRoots){
        nodes.insert(dist(gen));
    }
    return nodes;
}
