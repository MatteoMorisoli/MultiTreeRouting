//
//  CenterHeuristic.hpp
//  MultiTreeRouting
//
//  Created by Matteo Morisoli on 21.03.18.
//  Copyright © 2018 MatteoMorisoli. All rights reserved.
//

#ifndef CenterHeuristic_hpp
#define CenterHeuristic_hpp

#include <stdio.h>
#include <iostream>
#include "Heuristic.hpp"

class CenterHeuristic: public Heuristic{
public:
    std::set<int> selectStartingNodes(const int numVertices, const Graph& graph, const int numRoots) const;
};
#endif /* CenterHeuristic_hpp */
