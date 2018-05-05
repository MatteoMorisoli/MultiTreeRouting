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

//this heuristic will return the numRoots distinct vertices with the highest number of other vertices that are at most at distance 2 from them
class CenterHeuristic: public Heuristic{
public:
    CenterHeuristic(int numV);
    int selectStartingNode(const Graph& graph);
};
#endif /* CenterHeuristic_hpp */
